#pragma once

namespace Game {
	struct Skill_MoveToPlayer : Skill {
		float keepDistance{};

		void Init(Creature* creature_, float keepDistance_ = 0);
		int32_t Update() override;
	};
}
