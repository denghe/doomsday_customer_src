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

		// todo:
		// statCfg.init.xxxxxx = ?????
		StatCalc();
		healthPoint = sp.healthPoint;

		// add init skill
		skills.Emplace().Emplace<Skill_Control>()->Init(this);
		skills.Emplace().Emplace<Skill_Shoot_1>()->Init(this);
	}

	inline int32_t Player_3::Update() {
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
