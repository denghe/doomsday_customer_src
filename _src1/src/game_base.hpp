#pragma once

namespace Game {

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
}
