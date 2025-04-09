#pragma once

namespace Game {

	struct Skill_Shoot_1 : Skill {
		float shootCountPool{};		// time pool for shoot
		float aimRange{};           // cell size * 10 ?
		float radius{};             // == res.size.x
		float damage{};
		float moveSpeed{};          // ??? / fps
		float shootSpeed{};         // times per seconds / fps
		int32_t life{};             // seconds * fps
		int32_t pierceCount{};
		int32_t pierceDelay{};      // seconds * fps

		void Init(Creature* creature_);
		int32_t Update() override;
	};
}
