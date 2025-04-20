#pragma once

namespace Game {

	inline void Skill_Shoot_2::Init(Creature* creature_) {
		creature = creature_;
		stage = creature->stage;
		aimRange = Cfg::unitSize * 10;
		radius = ResTpFrames::_size_monster_hamburger.x;
		damage = 5;
		moveSpeed = 800.f / Cfg::fps;
		life = 3 * (int32_t)Cfg::fps;
	}

	inline int32_t Skill_Shoot_2::Update() {

		XX_BEGIN(n);
		if (stage->time < i) {
			XX_YIELD_I_TO_BEGIN(n);
		}

		{
			auto player = stage->player;
			auto d = player->pos - creature->pos;
			auto dd = d.x * d.x + d.y * d.y;
			auto mag = std::sqrtf(dd);
			if (mag > aimRange) {
				XX_YIELD_I_TO_BEGIN(n);
			}
			auto norm = d / mag;
			auto pos = creature->pos + creature->frame->spriteSize * norm; //bullet init pos
			auto a = std::atan2f(d.y, d.x);
			auto cos = std::cosf(a);
			auto sin = std::sinf(a);
			creature->state = State::ShootDelay;
			i = stage->time + cShootDelayFrames;
			stage->monsterBullets.Emplace().Emplace<Bullet_2>()->Init(this, pos, 0, 3, cos, sin);
		}

		while (i > stage->time) {
			xx::CoutN("ShootDelay");
			XX_YIELD_I(n);
		}
		i = stage->time + cCastDelayFrames;
		creature->state = State::Idle;
		XX_YIELD_I_TO_BEGIN(n);
		XX_END(n);
		return 0;
	}
}
