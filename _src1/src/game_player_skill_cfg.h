#pragma once

namespace Game {

	struct PlayerSkillCfg {
        float aimRange{};           // cell size * 10 ?
        float radius{};             // == res.size.x
        int32_t damage{};
        float moveSpeed{};          // ??? / fps
        float shootSpeed{};         // times per seconds / fps
        int32_t life{};             // seconds * fps
        int32_t pierceCount{};
        int32_t pierceDelay{};      // seconds * fps

		// todo ...
        float knockbackForce{};
	};

}
