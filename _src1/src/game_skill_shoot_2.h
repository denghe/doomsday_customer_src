#pragma once

namespace Game {
	// shoot player
	struct Skill_Shoot_2 : Skill {
		float shootCountPool{};		// time pool for shoot
		float aimRange{};           // cell size * 10 ?
		float radius{};             // == res.size.x
		float damage{};
		float moveSpeed{};          // ??? / fps
		float shootSpeed{};         // times per seconds / fps
		int32_t life{};             // seconds * fps

		void Init(Creature* creature_);
		int32_t Update() override;
	};
}
