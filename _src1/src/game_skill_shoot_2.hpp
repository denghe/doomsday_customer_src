#pragma once

namespace Game {

	inline void Skill_Shoot_2::Init(Creature* creature_) {
		creature = creature_;

		aimRange = Cfg::unitSize * 20;
		radius = ResTpFrames::_size_monster_hamburger.x * 0.5f;
		damage = 5;
		moveSpeed = 800.f / Cfg::fps;
		shootSpeed = 2 / Cfg::fps;
		life = 3 * (int32_t)Cfg::fps;
	}

	inline int32_t Skill_Shoot_2::Update() {
		shootCountPool += shootSpeed;
		if (auto count = (int)shootCountPool; count > 0) {
			shootCountPool -= count;
			auto pp = creature->pos + creature->frame->spriteSize * creature->scale * creature->offsetRatio;
			auto stage = creature->stage;
			auto player = stage->player;
			auto d = player->pos - pp;
			auto dd = d.x * d.x + d.y * d.y;
			auto mag = std::sqrtf(dd);
			if (mag < aimRange) {
				for (int i = 0; i < count; i++) {
					auto speedStep = moveSpeed / count;
					auto a = std::atan2f(d.y, d.x);
					auto cos = std::cosf(a);
					auto sin = std::sinf(a);
					auto r = (creature->frame->spriteSize.x * 0.5f - speedStep * i) * creature->scale.x;
					auto pos = pp + XY{ cos * r, sin * r };
					stage->monsterBullets.Emplace().Emplace<Bullet_2>()->Init(this, pos, 0, 3, cos, sin);
				}
			}
		}
		return 0;
	}
}
