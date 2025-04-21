#pragma once

namespace Game {

	void Monster_Chips::Init(Stage* stage_, StatCfg const& statCfg_, XY const& pos_) {
		stage = stage_;
		frame = gLooper.res.monster_chips;
		state = State::Idle;

		pos = pos_;
		scale = { 1,1 };
		radius = frame->spriteSize.x * 0.5f;
		radians = 0;

		statCfg = statCfg_;
		StatCalc();
		healthPoint = sp.healthPoint * sp.healthRatio;

		whiteColorEndTime = 0;
		destroyTime = stage->time + (int32_t)Cfg::fps * 60 * 5;

		tarOffsetRadius = frame->spriteSize.x * 3;
		tarOffset = stage->GetRndPosDoughnut(tarOffsetRadius, 0.1f);

		skills.Emplace().Emplace<Skill_MoveToPlayer>()->Init(this);
		skills.Emplace().Emplace<Skill_DashAttack>()->Init(this);
	}

	int32_t Monster_Chips::Update() {
		// auto use skill
		for (auto i = skills.len - 1; i >= 0; --i) {
			auto& skill = skills[i];
			if (skill->Update()) {
				skills.SwapRemoveAt(i);
			}
		}

		Idle();

		//auto p = stage->player.pointer;
		//xx::XY pp{};
		//xx::XYf d{};
		//float dd{};
		//float r2{};
		//float mag{};
		//xx::XYf norm{};
		//switch (state)
		//{
		//case State::Idle:
		//	// move to player
		//	Idle();
			//MoveToPlayer();
		//	break;
		//case State::Dash:
		//	MoveToPosition(p->pos,p->radius);
		//	break;
		//case State::Knockback:
		//	knockbackSpeed -= knockbackReduceValuePerFrame;
		//	if (knockbackSpeed <= 0) {
		//		ExitKnockback();
		//	}
		//	else {
		//		pos = pos + knockbackDist * knockbackSpeed;
		//		stage->ForceLimit(pos);
		//		stage->monsters.Update(this);	// sync space index
		//	}
		//	break;
		//default:
		//	Idle();
		//	break;
		//}

		// todo
		return destroyTime <= stage->time;
	}

	void Monster_Chips::ExitKnockback() {
		knockback = false;
		state = State::Idle;
	}

	void Monster_Chips::ExitDash() {
		state = State::Idle;
	}
}
