#pragma once

namespace Game {
	void Skill_MoveToPlayer::Init(Creature* creature_, float aimRange_) {
		creature = creature_;

		auto p = creature->stage->player;
		keepDistance = aimRange_ - p->radius - creature->radius;
	}

	int32_t Skill_MoveToPlayer::Update() {
		if (creature->state == State::Idle) {
			((Monster*)creature)->MoveToPlayer(keepDistance);
		}
		return 0;
	}
}
