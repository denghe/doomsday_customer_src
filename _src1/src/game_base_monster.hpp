#pragma once

namespace Game {

	inline void Monster::Knockback(float speed, XY const& d) {
		knockback = true;
		knockbackSpeed = speed * (1.f / Cfg::fps);
		auto dd = d.x * d.x + d.y * d.y;
		auto mag = std::sqrt(dd);
		knockbackDist = d / mag;
		knockbackReduceValuePerFrame = knockbackSpeed * (1.f / (Cfg::fps * 0.5f));
	}

	inline int32_t Monster::Update() {
		if (knockback) {
			knockbackSpeed -= knockbackReduceValuePerFrame;
			if (knockbackSpeed <= 0) {
				knockback = false;
			}
			else {
				pos = pos + knockbackDist * knockbackSpeed;
				stage->ForceLimit(pos);
				stage->monsters.Update(this);	// sync space index
			}
		}
		else {
			// move to player
			auto p = stage->player.pointer;
			auto pp = p->pos;
			auto d = pp - pos;
			auto dd = d.x * d.x + d.y * d.y;
			auto r2 = p->radius + radius;
			if (dd < r2 * r2) {
				// cross with player?
				//p->Hurt(damage, d);   // todo
			}
			else {
				d = pp - pos + tarOffset;
				auto dd = d.x * d.x + d.y * d.y;
				if (dd < radius * radius) {
					// reached offset point? reset offset point
					tarOffset = stage->GetRndPosDoughnut(tarOffsetRadius, 0.1f);
				}
				// calc & move
#if 0
				// slowly than mag normalize
				auto r = std::atan2f(d.y, d.x);
				pos += XY{ std::cosf(r) * moveSpeed, std::sinf(r) * moveSpeed };
#else
				// faster than atan2 + sin cos  1/4
				auto mag = std::sqrtf(dd);
				auto norm = d / mag;
				pos += norm * movementSpeedPerFrame;
#endif
				stage->ForceLimit(pos);
				stage->monsters.Update(this);	// sync space index
			}

			Idle();	// always play this anim
		}

		StatCalc();

		// todo
		return destroyTime <= stage->time;
	}

	inline int32_t Monster::Hurt(float dmg, XY const& txtD, XY const& knockbackD, bool isCrit) {
		if (dodgeChance > 0.f && stage->rnd.Next<float>(1) < dodgeChance) {
			// todo: show miss txt?
			return 0;
		}
		dmg = std::ceilf(dmg * defenseRatio);
		if (healthPoint <= dmg) {
			// dead
			stage->effectTexts.Add(pos + frame->spriteSize * XY{ 0, -0.5f }, txtD, xx::RGBA8_Red, 6, healthPoint);
			stage->effects.Emplace().Emplace<EffectDeath>()->Init(stage, frame, pos);
			stage->monsters.Remove(this);
			return 1;
		}
		else {
			// hurt
			healthPoint -= dmg;
			stage->effectTexts.Add(pos + frame->spriteSize * XY{ 0, -0.5f }, txtD
				, isCrit ? xx::RGBA8_Yellow : xx::RGBA8_White
				, 5, dmg);
			whiteColorEndTime = stage->time + int32_t(0.1f * Cfg::fps);
			Knockback(500.f, knockbackD);
			return 0;
		}
	}

	inline int32_t Monster::MoveToPosition(xx::XY targetPos, float targetRadius) {
		auto d = targetPos - pos;
		auto dd = d.x * d.x + d.y * d.y;
		auto r2 = targetRadius + radius;

		if (dd < r2 * r2) {
			return 1;
		}

		auto mag = std::sqrtf(dd);
		auto norm = d / mag;
		pos += norm * movementSpeedPerFrame;
		stage->ForceLimit(pos);
		stage->monsters.Update(this);	// sync space index
		return 0;
	}

	inline int32_t Monster::MoveToPlayer(float keepDistance) {
		auto p = stage->player;
		auto pp = p->pos;
		auto d = pp - pos;
		auto dd = d.x * d.x + d.y * d.y;
		auto r2 = p->radius + radius + keepDistance;
		if (dd < r2 * r2) {
			return 1;
		}

		d = pp - pos + tarOffset;
		dd = d.x * d.x + d.y * d.y;
		if (dd < radius * radius) {
			// reached offset point? reset offset point
			tarOffset = stage->GetRndPosDoughnut(tarOffsetRadius, 0.1f);
		}
		// calc & move
#if 0
		// slowly than mag normalize
		auto r = std::atan2f(d.y, d.x);
		pos += XY{ std::cosf(r) * moveSpeed, std::sinf(r) * moveSpeed };
#else
				// faster than atan2 + sin cos  1/4
		auto mag = std::sqrtf(dd);
		auto norm = d / mag;
		pos += norm * movementSpeedPerFrame;
#endif
		stage->ForceLimit(pos);
		stage->monsters.Update(this);	// sync space index
		return 0;
	}
}
