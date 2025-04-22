#pragma once

namespace Game {

	inline void Skill_Shoot_1::Init(Creature* creature_) {
		creature = creature_;

		aimRange = Cfg::unitSize * 20;
		radius = ResTpFrames::_size_bullet_coin5.x * 0.5f;
		damage = 5;
		moveSpeed = 800.f / Cfg::fps;
		shootSpeed = 3 / Cfg::fps;
		life = 3 * (int32_t)Cfg::fps;
		pierceCount = 0;
		pierceDelay = 0;
	}

	inline int32_t Skill_Shoot_1::Update() {
		shootCountPool += shootSpeed;
		if (auto count = (int)shootCountPool; count > 0) {
			shootCountPool -= count;
			auto pp = creature->pos + creature->frame->spriteSize * creature->scale * creature->offsetRatio;
			auto stage = creature->stage;
			// shoot nearest one
			if (auto o = stage->monsters.FindNearestByRange(pp.x, pp.y, aimRange)) {
				auto speedStep = moveSpeed / count;
				for (int i = 0; i < count; ++i) {
					auto d = o->pos - pp;
					auto a = std::atan2f(d.y, d.x);
					auto cos = std::cosf(a);
					auto sin = std::sinf(a);
					auto r = (creature->frame->spriteSize.x * 0.5f - speedStep * i) * creature->scale.x;
					auto pos = pp + XY{ cos * r, sin * r };
					stage->playerBullets.Emplace().Emplace<Bullet_1>()->Init(this, pos, 0, 3, cos, sin);
				}
			}
		}
		return 0;
	}
}
