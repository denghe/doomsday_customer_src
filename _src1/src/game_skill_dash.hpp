﻿#pragma once

namespace Game {
	inline void Skill_DashAttack::Init(Creature* creature_) {
		creature = creature_;
	}

	inline int32_t Skill_DashAttack::Update() {
		auto stage = creature->stage;

		XX_BEGIN(n);
		if (stage->time < i) {
			XX_YIELD_I_TO_BEGIN(n);
		}

		{
			auto p = stage->player.pointer;
			auto d = p->pos - creature->pos;
			auto dd = d.x * d.x + d.y * d.y;
			auto aimRange = cAimRangeScale * creature->frame->spriteSize.x;
			if (dd > aimRange * aimRange) {
				XX_YIELD_I_TO_BEGIN(n);
			}
			auto mag = std::sqrtf(dd);
			auto norm = d / mag;
			vec = norm * creature->movementSpeedPerFrame * cSpeedScale;
			creature->state = State::PreDash;
			i = stage->time + cDashDelayFrames;
			auto length = cSpeedScale * creature->movementSpeedPerFrame * cDurationFrames;
			stage->effects.Emplace().Emplace<WarningDash>()->Init(stage, creature->pos, p->pos, { length,creature->frame->spriteSize.x }, cDashDelaySeconds, 1.f);
		}

		c = 1.f;
		while (i > stage->time) {
			creature->color = xx::RGBA8{ 255,uint8_t(255 * c),uint8_t(255 * c),255 };
			c -= cColorStepDashDelay;
			XX_YIELD_I(n);
		}
		creature->state = State::Dashing;
		c = 0;
		for (i = 0;i < cDurationFrames; i++) {
			creature->color = xx::RGBA8{ 255,uint8_t(255 * c),uint8_t(255 * c),255 };
			c += cColorStepReset;

			creature->pos += vec;
			stage->ForceLimit(creature->pos);
			stage->monsters.Update((Monster*)creature);	// sync space index
			XX_YIELD_I(n);
		}

		creature->color = xx::RGBA8_White;
		i = stage->time + cCastDelayFrames;
		creature->state = State::Idle;
		XX_YIELD_I_TO_BEGIN(n);
		XX_END(n);
		return 0;
	}
}
