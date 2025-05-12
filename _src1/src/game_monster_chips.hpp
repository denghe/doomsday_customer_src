#pragma once

namespace Game {

	void Monster_Chips::Init(Stage* stage_, XY const& pos_) {
		creatureType = CreatureTypes::Monster_Chip;
		stage = stage_;
		frame = gLooper.res.monster_chips;
		state = State::Idle;

		pos = pos_;
		scale = { 1,1 };
		radius = frame->spriteSize.x * 0.5f;
		radians = 0;

		// todo: fill init stat
		healthPointMax = Stat_t(stage->roundId * 6 + 20);
		StatCalc();
		healthPoint = healthPointMax;

		whiteColorEndTime = 0;
		destroyTime = stage->time + (int32_t)Cfg::fps * 60 * 5;

		tarOffsetRadius = frame->spriteSize.x * 3;
		tarOffset = stage->GetRndPosDoughnut(tarOffsetRadius, 0.1f);

		skills.Emplace().Emplace<Skill_MoveToPlayer>()->Init(this);
		//skills.Emplace().Emplace<Skill_DashAttack>()->Init(this);
		skills.Emplace().Emplace<Skill_Attack>()->Init(this);
	}

	int32_t Monster_Chips::Update() {
		// auto use skill
		tagPos = { pos.x - frame->spriteSize.x * .1f, pos.y + frame->spriteSize.y * 0.1f };
		for (auto i = skills.len - 1; i >= 0; --i) {
			auto& skill = skills[i];
			if (skill->Update()) {
				skills.SwapRemoveAt(i);
			}
		}

		Idle();

		return destroyTime <= stage->time;
	}

	void Monster_Chips::Draw() {
		Monster::Draw();
		DrawTag();
	}

	void Monster_Chips::DrawTag() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(gLooper.res.ef_atkdmg->tex, 1);
		XY s{ scale * stage->camera.scale };
		if (needFlipX) s.x = -s.x;
		// body
		q[0].pos = stage->camera.ToGLPos(tagPos);
		q[0].anchor = frame->anchor.has_value() ? *frame->anchor : XY{ 0.5f, 0.5f };
		q[0].scale = s * 2.f;
		q[0].radians = 0;
		q[0].colorplus = whiteColorEndTime >= stage->time ? 10000.f : 1.f;
		q[0].color = color;
		q[0].texRect.data = gLooper.res._uvrect_ef_atkdmg.data;
	}

	void Monster_Chips::Rewards() {
		auto rewardIdx = gLooper.rnd.Next<int>(0, 3);
		auto loot = xx::MakeShared<Chip_Loot>();
		loot->Init(stage);
		loot->pos = pos;
		loot->coin = 5;

		switch (rewardIdx)
		{
		case 0:
			loot->healthPoint = 10.f;
			break;
		case 1:
			loot->damage = 1.f;
			break;
		case 2:
			loot->attackSpeed = .5f;
			break;
		default:
			break;
		}

		stage->loots.Add(std::move(loot));
	}

	void Chip_Loot::Init(Stage* stage_) {
		stage = stage_;
		type = LootTypes::Chip;
	}

	void Chip_Loot::Collect(Creature* owner) {
		if (coin != 0) {
			owner->coin += coin;
		}

		if (damage != 0) {
			owner->UpdateDamage(damage);
		}

		if (attackSpeed != 0) {
			owner->UpdateAttackSpeed(attackSpeed);
		}

		if (healthPoint != 0) {
			owner->UpdateHealthPoint(healthPoint);
		}
	}
}
