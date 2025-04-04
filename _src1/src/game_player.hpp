#pragma once

namespace Game {

	inline void Player_1::Init(Stage* stage_) {
		stage = stage_;
		frame = gLooper.res.player1;

		pos = stage_->GetPlayerBornPos();
		scale = { 1,1 };
		radius = ResTpFrames::_size_player1.x * 0.5f;
		radians = 0;

		hp = 100;
		damage = 1;
		moveSpeed = 500.f / Cfg::fps;
		criticalRate = 0.1f;
		criticalDamageRatio = 2;

		// add init skill for test
		skills.Emplace().Emplace<Skill_1>()->Init(this, stage->skillCfgs[0]);
	}

	inline int32_t Player_1::Update() {
		// move control
		if (auto inc = gLooper.GetKeyboardMoveInc(); inc.has_value()) {
			pos += inc->second * moveSpeed;
			stage->ForceLimit(pos);	// for safe
			// todo: more block limit
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


	/********************************************************************************************************************/
	/********************************************************************************************************************/


	inline void Player_2::Init(Stage* stage_) {
		stage = stage_;
		frame = gLooper.res.player2;

		pos = stage_->GetPlayerBornPos();
		scale = { 1,1 };
		radius = ResTpFrames::_size_player2.x * 0.5f;
		radians = 0;

		hp = 100;
		damage = 1;
		moveSpeed = 500.f / Cfg::fps;
		criticalRate = 0.1f;
		criticalDamageRatio = 2;

		// add init skill for test
		skills.Emplace().Emplace<Skill_1>()->Init(this, stage->skillCfgs[0]);
	}

	inline int32_t Player_2::Update() {
		// move control
		auto posBak = pos;
		if (auto inc = gLooper.GetKeyboardMoveInc(); inc.has_value()) {
			pos += inc->second * moveSpeed;
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
