#pragma once

namespace Game {
	// shoot player
	struct Skill_Shoot_2 : Skill {
		static constexpr float cInitScale{ .5f };
		static constexpr float cShootDelaySeconds{ 3.f };
		static constexpr int32_t cShootDelayFrames{ int32_t(cShootDelaySeconds * Cfg::fps) };
		static constexpr float cShootDelayScaleStep{ (1.f - cInitScale) / cShootDelayFrames };
		static constexpr float cCastDelaySeconds{ 10.f };
		static constexpr int32_t cCastDelayFrames{ int32_t(cCastDelaySeconds * Cfg::fps) };

		float aimRange{};           // cell size * 10 ?
		float radius{};             // == res.size.x
		float damage{};
		float moveSpeed{};          // ??? / fps
		int32_t life{};             // seconds * fps

		int32_t i{};
		int32_t n{};

		Stage* stage{};

		void Init(Creature* creature_);
		int32_t Update() override;
	};
}
