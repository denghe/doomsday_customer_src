#pragma once

namespace Game {
	void Skill_MoveToPlayer::Init(Creature* creature_) {
		creature = creature_;
	}

	int32_t Skill_MoveToPlayer::Update() {
		if (creature->state == State::Idle) {
			((Monster*)creature)->MoveToPlayer();
		}
		return 0;
	}
}
