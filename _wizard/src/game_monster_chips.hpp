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

		tagId = gLooper.rnd.Next<int>(1, 4);
		switch (tagId)
		{
		case 1:
			tagFrame = gLooper.res.ef_heal;
			break;
		case 2:
			tagFrame = gLooper.res.ef_atkdmg;
			break;
		case 3:
			tagFrame = gLooper.res.ef_atkspd;
			break;
		default:
			break;
		}
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

		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(tagFrame->tex, 1);
		XY s{ scale * stage->camera.scale };
		if (needFlipX) s.x = -s.x;
		// body
		q[0].pos = stage->camera.ToGLPos(tagPos);
		q[0].anchor = frame->anchor.has_value() ? *frame->anchor : XY{ 0.5f, 0.5f };
		q[0].scale = s * 2.f;
		q[0].radians = 0;
		q[0].colorplus = whiteColorEndTime >= stage->time ? 10000.f : 1.f;
		q[0].color = color;
		q[0].texRect.data = tagFrame->textureRect.data;
	}

	void Monster_Chips::Rewards() {
		auto loot = xx::MakeShared<Loot>();
		loot->Init(this);
		stage->loots.Add(std::move(loot));
	}
}
