#pragma once

namespace Game {

	int32_t Monster_Cola::Hurt(float dmg, XY const& d) {
		dmg = std::ceilf(dmg);
		if (hp <= dmg) {
			// dead
			stage->etm.Add(pos + ResTpFrames::_size_monster_chips * XY{ 0, -0.5f }, d, xx::RGBA8_Red, 6, hp);
			// todo: death effect
			stage->monsters.Remove(this);
			return 1;
		}
		else {
			// hurt
			hp -= dmg;
			stage->etm.Add(pos + ResTpFrames::_size_monster_chips * XY{ 0, -0.5f }, d, xx::RGBA8_Yellow, 5, dmg);
			whiteColorEndTime = stage->time + int32_t(0.1f / Cfg::fps);
			return 0;
		}
	}

	void Monster_Cola::Init(Stage* stage_, XY const& pos_) {
		stage = stage_;
		pos = pos_;
		scale = { 1,1 };
		radius = ResTpFrames::_size_monster_chips.x * 0.5f;
		radians = 0;

		hp = 30.f + stage->round * 8;
		damage = 1;
		moveSpeed = 300.f / Cfg::fps;
		criticalRate = 0.1f;
		criticalDamageRatio = 2;

		whiteColorEndTime = 0;
		destroyTime = stage->time + (int32_t)Cfg::fps * 60 * 5;

		tarOffsetRadius = ResTpFrames::_size_monster_chips.x * 3;
		tarOffset = stage->GetRndPosDoughnut(tarOffsetRadius, 0.1f);
	}

	int32_t Monster_Cola::Update() {
		auto p = stage->player.pointer;
		auto pp = p->pos;
		auto d = pp - pos;
		auto dd = d.x * d.x + d.y * d.y;
		auto r2 = p->radius + radius;
		if (dd < r2 * r2) {
			// cross with player?
			p->Hurt(damage, d);
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

	void Monster_Cola::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res._texid_monster_cola, 1);
		q->pos = stage->camera.ToGLPos(pos);
		q->anchor = ResTpFrames::_anchor_monster_cola;
		q->scale = scale * stage->camera.scale;
		q->radians = 0;
		q->colorplus = whiteColorEndTime >= stage->time ? 10000.f : 1.f;
		q->color = xx::RGBA8_White;
		q->texRect.data = ResTpFrames::_uvrect_monster_cola.data;
	}

}
