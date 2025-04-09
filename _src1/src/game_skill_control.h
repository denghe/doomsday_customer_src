#pragma once

namespace Game {
	// for player ASDW move
	struct Skill_Control : Skill {
		void Init(Creature* creature_);
		int32_t Update() override;
	};
}
