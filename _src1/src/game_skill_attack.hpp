#pragma once

namespace Game {
	inline void Skill_Attack::Init(Creature* creature_) {
		creature = creature_;
		attackRange = creature->radius + creature->stage->player->radius;
	}

	inline int32_t Skill_Attack::Update() {
		auto stage = creature->stage;

		XX_BEGIN(n);
		if (stage->time < i) {
			XX_YIELD_I_TO_BEGIN(n);
		}

		{
			auto p = stage->player.pointer;
			auto d = p->pos - creature->pos;
			auto dd = d.x * d.x + d.y * d.y;
			if (dd > attackRange * attackRange) {
				XX_YIELD_I_TO_BEGIN(n);
			}
			p->Hurt();
			i = stage->time + cDurationFrames;
			creature->color = xx::RGBA8_Red;
		}

		while (i > stage->time) {
			XX_YIELD_I(n);
		}

		creature->color = xx::RGBA8_White;
		i = stage->time + cCastDelayFrames;
		XX_YIELD_I_TO_BEGIN(n);
		XX_END(n);
		return 0;
	}
}