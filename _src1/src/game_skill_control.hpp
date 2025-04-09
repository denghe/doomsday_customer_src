#pragma once

namespace Game {
	void Skill_Control::Init(Creature* creature_) {
		creature = creature_;
	}

	int32_t Skill_Control::Update() {
		auto c = creature;
		auto s = creature->stage;

		if (c->castingSkillCount) return 0;	// can't move when casting

		auto posBak = c->pos;
		if (auto inc = gLooper.GetKeyboardMoveInc(); inc.has_value()) {
			c->pos += inc->second * c->movementSpeedPerFrame;
			c->stage->ForceLimit(c->pos);
			// todo: more block limit
		}
		if (posBak != c->pos) {
			// moved
			if (posBak.x > c->pos.x) c->needFlipX = true;
			else if (posBak.x < c->pos.x) c->needFlipX = false;
		}

		// always
		c->Idle();

		return 0;
	}
}
