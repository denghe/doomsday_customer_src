#pragma once

namespace Game {

	inline void Drawable::Draw() {
		//auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(frame->tex, 1);
		//q->pos = stage->camera.ToGLPos(pos);
		//q->anchor = frame->anchor.has_value() ? *frame->anchor : XY{ 0.5f, 0.5f };
		//q->scale = scale * stage->camera.scale;
		//q->radians = 0;
		//q->colorplus = whiteColorEndTime >= stage->time ? 10000.f : 1.f;
		//q->color = xx::RGBA8_White;
		//q->texRect.data = frame->textureRect.data;

		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(frame->tex, 2);
		XY s{ scale * stage->camera.scale };
		if (needFlipX) s.x = -s.x;
		// body
		q[1].pos = stage->camera.ToGLPos(pos);
		q[1].anchor = frame->anchor.has_value() ? *frame->anchor : XY{ 0.5f, 0.5f };
		q[1].scale = s;
		q[1].radians = 0;
		q[1].colorplus = whiteColorEndTime >= stage->time ? 10000.f : 1.f;
		q[1].color = xx::RGBA8_White;
		q[1].texRect.data = frame->textureRect.data;
		// shadow
		q[0].scale = { q[1].scale.x, q[1].scale.y * 0.2f };
		q[0].pos = { q[1].pos.x, q[1].pos.y - q[0].scale.y * frame->spriteSize.y * 0.5f };
		q[0].anchor = { q[1].anchor.x, 0.3 };
		q[0].radians = q[1].radians;
		q[0].colorplus = q[1].colorplus;
		q[0].color = { 0,0,0,127 };
		q[0].texRect.data = q[1].texRect.data;
	}

	inline XY Stage::GetPlayerBornPos() {
		return mapSize * 0.5f;
	}

	inline void Stage::ForceLimit(XY& pos) {
		if (pos.x < 0.f) pos.x = 0.f;
		else if (pos.x >= mapSize.x)
			pos.x = mapSize.x - std::numeric_limits<float>::epsilon();
		if (pos.y < 0.f) pos.y = 0.f;
		else if (pos.y >= mapSize.y)
			pos.y = mapSize.y - std::numeric_limits<float>::epsilon();
	}

	inline bool Stage::IsOutOfMap(XY const& pos) {
		return pos.x < 0 || pos.y < 0 || pos.x >= mapSize.x || pos.y >= mapSize.y;
	}

	inline XY Stage::GetRndPosDoughnut(float maxRadius, float safeRadius) {
		auto len = maxRadius - safeRadius;
		auto len_radius = len / maxRadius;
		auto safeRadius_radius = safeRadius / maxRadius;
		auto radius = std::sqrtf(rnd.Next<float>(0, len_radius) + safeRadius_radius) * maxRadius;
		auto radians = rnd.Next<float>(-M_PI, M_PI);
		return { std::cosf(radians) * radius, std::sinf(radians) * radius };
	}

	inline void Creature::Idle() {
		XX_BEGIN(idle_lineNumber);
		for (scale.y = 1.f; scale.y >= IdleCfg::cEnd; scale.y -= IdleCfg::cInc) {
			scale.x = IdleCfg::cTotal - scale.y;
			XX_YIELD(idle_lineNumber);
		}
		for (; scale.y <= 1.f; scale.y += IdleCfg::cInc) {
			scale.x = IdleCfg::cTotal - scale.y;
			XX_YIELD(idle_lineNumber);
		}
		XX_YIELD_TO_BEGIN(idle_lineNumber);
		XX_END(idle_lineNumber);
	}

	inline void MonsterGen::Init(Stage* stage_, int activeTime_, int destroyTime_, float generateNumsPerSeconds_) {
		stage = stage_;
		activeTime = activeTime_;
		destroyTime = destroyTime_;
		countIncPerFrame = generateNumsPerSeconds_ / Cfg::fps;
	}

	int32_t Monster::Hurt(float dmg, XY const& d) {
		dmg = std::ceilf(dmg);
		if (hp <= dmg) {
			// dead
			stage->etm.Add(pos + frame->spriteSize * XY{ 0, -0.5f }, d, xx::RGBA8_Red, 6, hp);
			stage->effects.Emplace().Emplace<EffectDeath>()->Init(stage, frame, pos);
			stage->monsters.Remove(this);
			return 1;
		}
		else {
			// hurt
			hp -= dmg;
			stage->etm.Add(pos + frame->spriteSize * XY{ 0, -0.5f }, d, xx::RGBA8_Yellow, 5, dmg);
			whiteColorEndTime = stage->time + int32_t(0.1f * Cfg::fps);
			return 0;
		}
	}
}
