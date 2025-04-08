#pragma once

namespace Game {
	struct Skill_MoveToPlayer : Skill {
		void Init(Creature* creature_);
		int32_t Update() override;
	};
}
