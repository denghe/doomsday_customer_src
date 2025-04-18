#pragma once

namespace Game {
	struct Skill_DashAttack : Skill {
		static constexpr float cCastDelaySeconds{ 10.f };
		static constexpr int32_t cCastDelayFrames{ int32_t(cCastDelaySeconds * Cfg::fps) };
		static constexpr float cDashDelaySeconds{ 1.f };
		static constexpr int32_t cDashDelayFrames{ int32_t(cDashDelaySeconds * Cfg::fps) };
		static constexpr float cSpeedScale{ 5 };
		static constexpr float cDurationSeconds{ 1.f };
		static constexpr int32_t cDurationFrames{ int32_t(cDurationSeconds * Cfg::fps) };
		static constexpr float cAimRangeScale{ 10.f };
		static constexpr float cColorStepDashDelay{ 1.f / cDashDelayFrames };
		static constexpr float cColorStepReset{ 1.f / cDurationFrames };

		int32_t i{};
		int32_t n{};
		xx::XY vec{};
		float c{};

		void Init(Creature* creature_);
		int32_t Update() override;
	};
}
