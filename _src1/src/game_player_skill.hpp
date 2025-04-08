#pragma once

namespace Game {

	inline void Skill_1::Init(Creature* creature_, SkillCfg* skillCfg_) {
		creature = creature_;
		cfg = skillCfg_;
	}

	inline int32_t Skill_1::Update() {
        shootCountPool += cfg->shootSpeed;
        if (auto count = (int)shootCountPool; count > 0) {
            shootCountPool -= count;
            auto pp = creature->pos + creature->frame->spriteSize * creature->scale * creature->offsetRatio;
            auto stage = creature->stage;
            // shoot nearest one
            if (auto o = stage->monsters.FindNearestByRange(pp.x, pp.y, cfg->aimRange)) {
                auto speedStep = cfg->moveSpeed / count;
                for (int i = 0; i < count; ++i) {
                    auto d = o->pos - pp;
                    auto a = std::atan2f(d.y, d.x);
                    auto cos = std::cosf(a);
                    auto sin = std::sinf(a);
                    auto r = (creature->frame->spriteSize.x * 0.5f - speedStep * i) * creature->scale.x;
                    auto pos = pp + XY{ cos * r, sin * r };
                    stage->playerBullets.Emplace().Emplace<PlayerBullet_1>()->Init(this, pos, 0, cos, sin);
                }
            }
        }
		return 0;
	}


	inline void Skill_DashAttack::Init(Creature* creature_, SkillCfg* skillCfg_) {
		creature = creature_;
		cfg = skillCfg_;
	}

	inline int32_t Skill_DashAttack::Update() {
		if (coolDownTimer) {
			coolDownTimer--;
		}
		else {   // auto use skill
			auto p = creature->stage->player.pointer;
			auto pp = p->pos;
			auto d = pp - creature->pos;
			auto dd = d.x * d.x + d.y * d.y;
			if (dd <= cfg->aimRange * cfg->aimRange) {
				creature->state = State::Dash;
				xx::CoutN("Enter Dash");
				defaultSpeed = creature->movementSpeedPerFrame;
				creature->movementSpeedPerFrame = cfg->moveSpeed;
				coolDownTimer = coolDown;
				endFrameNumber = creature->stage->time + cfg->life;
				xx::CoutN("defaultSpeed: ",defaultSpeed, ", currentSpeed: ", creature->movementSpeedPerFrame);
			}
		}

		if(creature->state == State::Dash && creature->stage->time >= endFrameNumber) {
			xx::CoutN("Exit Dash");
			creature->state = State::Idle;
			creature->movementSpeedPerFrame = defaultSpeed;
		}
		return 0;
	}
}
