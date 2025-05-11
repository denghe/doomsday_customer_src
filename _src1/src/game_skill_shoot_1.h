#pragma once

namespace Game {

	struct Skill_Shoot_1 : Skill {
		float shootCountPool{};		// time pool for shoot
		float aimRange{};           // cell size * 10 ?
		float radius{};             // == res.size.x
		float damage{};
		float moveSpeed{};          // ??? / fps
		float shootSpeed{};         // times per seconds / fps
		static constexpr float cProjectileRadian{ 15 * M_PI / 180.f };	// the space between trajectory
		static constexpr float cOffsetRadian{ cProjectileRadian / 2.f};
		int32_t projectileCount{};	// times per seconds / fps
		int32_t life{};             // seconds * fps
		int32_t pierceCount{};
		int32_t pierceDelay{};      // seconds * fps

		void Init(Creature* creature_);
		int32_t Update() override;
	};
}
