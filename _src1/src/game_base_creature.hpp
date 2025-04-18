﻿#pragma once

namespace Game {

	inline void Creature::StatCalc() {
		// gather points & results from items
		sp = statCfg.init;
		if (auto equipmentsCount = equipments.Count(); equipmentsCount > 0) {
			for (int32_t ei = 0; ei < equipmentsCount; ++ei) {
				auto& e = equipments[ei];
				if (auto statsCount = e->stats.Count(); statsCount > 0) {
					for (int32_t si = 0; si < statsCount; ++si) {
						auto& s = e->stats[si];
						sp[(uint32_t)s.type] += s.value;
					}
				}
			}
		}
		// calculate
		sp.healthRegeneration += sp.luckyPoint * statCfg.luckyToHealthRegenerationRatio;
		sp.damageRatio += sp.luckyPoint * statCfg.luckyToDamageRatio;
		sp.criticalChance += sp.luckyPoint * statCfg.luckyTocriticalChanceRatio;
		// apply rules
		for (int32_t i = 0; i < StatPanel::numFields; ++i) {
			auto& v = sp[i];
			if (v < statCfg.rangeFrom[i]) v = statCfg.rangeFrom[i];
			else if (v > statCfg.rangeTo[i]) v = statCfg.rangeTo[i];
		}
		// fill some final result
		defenseRatio = statCfg.defenseFactor / (statCfg.defenseFactor + sp.defensePoint);
		dodgeChance = sp.dodgePoint / (sp.dodgePoint + statCfg.dodgeFactor);
		movementSpeed = statCfg.baseMovementSpeed * sp.movementSpeedPoint;
		movementSpeedPerFrame = movementSpeed * Cfg::frameDelay;
	}

	inline void Creature::Idle() {
		XX_BEGIN(n_Idle);
		for (scale.y = 1.f; scale.y >= IdleCfg::cEnd; scale.y -= IdleCfg::cInc) {
			scale.x = IdleCfg::cTotal - scale.y;
			XX_YIELD(n_Idle);
		}
		for (; scale.y <= 1.f; scale.y += IdleCfg::cInc) {
			scale.x = IdleCfg::cTotal - scale.y;
			XX_YIELD(n_Idle);
		}
		XX_YIELD_TO_BEGIN(n_Idle);
		XX_END(n_Idle);
	}

}
