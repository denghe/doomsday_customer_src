#pragma once

namespace Game {
	struct Skill_Attack : Skill {
		static constexpr float cCastDelaySeconds{ 0.f };
		static constexpr int32_t cCastDelayFrames{ int32_t(cCastDelaySeconds * Cfg::fps) };
		static constexpr float cDurationSeconds{ 1.f };
		static constexpr int32_t cDurationFrames{ int32_t(cDurationSeconds * Cfg::fps) };

		int32_t i{};
		int32_t n{};
		float attackRange{};

		void Init(Creature* creature_);
		int32_t Update() override;
	};
}