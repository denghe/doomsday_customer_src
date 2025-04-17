﻿#pragma once

namespace Game {
	struct Skill_DashAttack : Skill {
		static constexpr int32_t cCastDelay{ (int32_t)Cfg::fps * 10 };
		static constexpr float cDashDelaySeconds{ 1.f };
		static constexpr int32_t cDashDelayFrames{ int32_t(cDashDelaySeconds * Cfg::fps) };
		static constexpr float cSpeedScale{ 5 };
		static constexpr int32_t cDuration { (int32_t)(Cfg::fps * 1.f) };
		static constexpr float cAimRangeScale{ 10 };
		static constexpr float cColorStep{ 1.f / cDashDelaySeconds };
		static constexpr float cColorStep2{ 1.f / cDuration };

		int32_t i{};
		int32_t n{};
		xx::XY vec{};
		float c{};

		void Init(Creature* creature_);
		int32_t Update() override;
	};
}
