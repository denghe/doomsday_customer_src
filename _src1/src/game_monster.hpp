#pragma once

namespace Game {

	void Monster_1::Init(Stage* stage_, XY const& pos_) {
		stage = stage_;
		pos = pos_;
		scale = { 1,1 };
		moveSpeed = 300.f / Cfg::fps;
		radius = ResTpFrames::_size_monster_chips_.x * 0.5f;

		tarOffsetRadius = ResTpFrames::_size_monster_chips_.x * 3;
		tarOffset = stage->GetRndPosDoughnut(tarOffsetRadius, 0.1f);

		damage = 1;
		criticalRate = 0.1f;
		criticalDamageRatio = 2;

		destroyTime = stage->time + (int32_t)Cfg::fps * 60 * 5;
	}

	int32_t Monster_1::Update() {
		auto p = stage->player.pointer;
		auto pp = p->pos;
		auto d = pp - pos;
		auto dd = d.x * d.x + d.y * d.y;
		auto r2 = p->radius + radius;
		if (dd < r2 * r2) {
			// cross with player?
			p->Hurt(damage);
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

	void Monster_1::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res.monster_chips->tex->GetValue(), 1);
		q->pos = stage->camera.ToGLPos(pos);
		q->anchor = ResTpFrames::_anchor_monster_chips_;
		q->scale = scale * Cfg::globalScale * stage->camera.scale;
		q->radians = 0;
		q->colorplus = 1;
		q->color = xx::RGBA8_White;
		q->texRect.data = gLooper.res.monster_chips->textureRect.data;
	}

}
