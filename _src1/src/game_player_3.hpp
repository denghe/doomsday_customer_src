#pragma once

namespace Game {

	inline void Player_3::Init(Stage* stage_) {
		stage = stage_;
		frame = gLooper.res.player2;

		pos = stage_->GetPlayerBornPos();
		scale = { 1,1 };
		offsetRatio = { 0.f, -0.3f };
		radius = ResTpFrames::_size_player2.x * 0.5f;
		radians = 0;

		// todo: init
		StatCalc();
		healthPoint = sp.healthPoint;

		// add init skill for test
		//skills.Emplace().Emplace<Skill_1>()->Init(this, stage->skillCfgs[0]);
	}

	inline int32_t Player_3::Update() {
		// move control
		auto posBak = pos;
		if (auto inc = gLooper.GetKeyboardMoveInc(); inc.has_value()) {
			pos += inc->second * movementSpeedPerFrame;
			stage->ForceLimit(pos);	// for safe
			// todo: more block limit
		}
		if (posBak != pos) {
			// moved
			if (posBak.x > pos.x) needFlipX = true;
			else if (posBak.x < pos.x) needFlipX = false;
		}

		// always
		Idle();

		// skills driven
		for (auto i = skills.len - 1; i >= 0; --i) {
			auto& skill = skills[i];
			if (skill->Update()) {
				skills.SwapRemoveAt(i);
			}
		}

		// if reutrn !0 mean player is dead
		return 0;
	}
}
