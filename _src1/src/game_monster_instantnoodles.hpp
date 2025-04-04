#pragma once

namespace Game {

	void Monster_Instantnoodles::Init(Stage* stage_, XY const& pos_) {
		stage = stage_;
		frame = gLooper.res.monster_instantnoodles;

		pos = pos_;
		scale = { 1,1 };
		radius = frame->spriteSize.x * 0.5f;
		radians = 0;

		hp = 15.f + stage->round * 5;
		damage = 1;
		moveSpeed = 300.f / Cfg::fps;
		criticalRate = 0.1f;
		criticalDamageRatio = 2;

		whiteColorEndTime = 0;
		destroyTime = stage->time + (int32_t)Cfg::fps * 60 * 5;

		tarOffsetRadius = frame->spriteSize.x * 3;
		tarOffset = stage->GetRndPosDoughnut(tarOffsetRadius, 0.1f);
	}

	int32_t Monster_Instantnoodles::Update() {
		auto p = stage->player.pointer;
		auto pp = p->pos;
		auto d = pp - pos;
		auto dd = d.x * d.x + d.y * d.y;
		auto r2 = p->radius + radius;
		if (dd < r2 * r2) {
			// cross with player?
			//p->Hurt(damage, d);
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
			auto r = std::atan2f(d.y, d.x);
			pos += XY{ std::cosf(r) * moveSpeed, std::sinf(r) * moveSpeed };
#else
			// faster than atan2 + sin cos  1/4
			auto mag = std::sqrt(dd);
			auto norm = d / mag;
			pos += norm * moveSpeed;
#endif
			stage->ForceLimit(pos);
			stage->monsters.Update(this);	// sync space index
		}

		// always
		Idle();

		// todo
		return destroyTime <= stage->time;
	}

}
