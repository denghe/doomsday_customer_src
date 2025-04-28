#pragma once

namespace Game {

	inline void Creature::StatCalc() {
		// gather points & results from buffs
		sp = statCfg.init;
		//for (int32_t i = 0; i < StatPanel::numFields; ++i) {
		//	sp[i] += buffs.sp[i];
		//}

		// calculate
		sp.healthRegeneration += sp.luckyPoint * statCfg.luckyToHealthRegenerationRatio;
		sp.damageRatio += sp.luckyPoint * statCfg.luckyToDamageRatio;
		sp.criticalChance += sp.luckyPoint * statCfg.luckyTocriticalChanceRatio;

		// apply limit rules
		for (int32_t i = 0; i < StatPanel::numFields; ++i) {
			auto& v = sp[i];
			if (v < statCfg.rangeFrom[i]) v = statCfg.rangeFrom[i];
			else if (v > statCfg.rangeTo[i]) v = statCfg.rangeTo[i];
		}

		// fill some final result
		healthPointMax = sp.healthPoint * sp.healthRatio;
		damageReduce = statCfg.defenseFactor / (statCfg.defenseFactor + sp.defensePoint);
		dodgeChance = sp.dodgePoint / (sp.dodgePoint + statCfg.dodgeFactor);
		movementSpeed = statCfg.baseMovementSpeed * sp.movementSpeedPoint;
		movementSpeedPerFrame = movementSpeed * Cfg::frameDelay;
	}

	inline void Creature::OnRoundBegin() {

		if (auto num = buffs.At(BuffTypes::技术债)) {
			buffs.sp.healthRatio -= 0.1f * num;
			buffs.sp.harvestRatio -= 0.1f * num;
		}

		// gather points & results from buffs
		sp = statCfg.init;
		for (int32_t i = 0; i < StatPanel::numFields; ++i) {
			sp[i] += buffs.sp[i];
		}

		// calculate
		sp.healthRegeneration += sp.luckyPoint * statCfg.luckyToHealthRegenerationRatio;
		if (auto num = buffs.At(BuffTypes::南无加特林机枪菩萨)) {
			sp.damageRatio += (sp.luckyPoint * 1.3f * num) * statCfg.luckyToDamageRatio;
		}
		else {
			sp.damageRatio += sp.luckyPoint * statCfg.luckyToDamageRatio;
		}
		sp.criticalChance += sp.luckyPoint * statCfg.luckyTocriticalChanceRatio;

		// apply limit rules
		for (int32_t i = 0; i < StatPanel::numFields; ++i) {
			auto& v = sp[i];
			if (v < statCfg.rangeFrom[i]) v = statCfg.rangeFrom[i];
			else if (v > statCfg.rangeTo[i]) v = statCfg.rangeTo[i];
		}

		// calc some final result
		if (auto num = buffs.At(BuffTypes::南无阿弥陀佛)) {
			healthPointMax = sp.healthPoint * sp.healthRatio + sp.luckyPoint * 3;
		}
		else {
			healthPointMax = sp.healthPoint * sp.healthRatio;
		}
		damageReduce = statCfg.defenseFactor / (statCfg.defenseFactor + sp.defensePoint);
		dodgeChance = sp.dodgePoint / (sp.dodgePoint + statCfg.dodgeFactor);
		movementSpeed = statCfg.baseMovementSpeed * sp.movementSpeedPoint;
		movementSpeedPerFrame = movementSpeed * Cfg::frameDelay;
		healthPoint = healthPointMax;


		if (auto num = buffs.At(BuffTypes::跑步机)) {
			if (!buffs._跑步机_used) {
				buffs._跑步机_used = true;
				healthPoint = healthPointMax * std::powf(0.3f, (float)num);
			}
		}

		if (auto num = buffs.At(BuffTypes::筋肉兄贵)) {
			if (!buffs._筋肉兄贵_used) {
				buffs._筋肉兄贵_used = true;
				healthPoint = healthPointMax * std::powf(0.01f, (float)num);
			}
		}

		// minval protect
		if (healthPoint < 1) {
			healthPoint = 1;
		}

		// round counters cleanup
		buffs._肉食主义_damageScaleCount = 0;
		buffs._裁员大动脉_usedCount = 0;
	}

	inline void Creature::OnRoundEnd() {
		if (auto num = buffs.At(BuffTypes::投资)) {
			if (stage->rnd.Next<bool>()) {
				coin *= 2;
				// todo: play effect?
			}
			else {
				coin *= 0.5f;
				// todo: play effect?
			}
		}
	}

	XX_INLINE void Creature::OnKilled(Creature* other) {

		if (auto num = buffs.At(BuffTypes::国潮外卖)) {
			if (stage->rnd.Next<float>() < 0.7f) {
				healthPoint += 30 * num;
				// todo: play effect?
				if (healthPoint > healthPointMax) {
					healthPoint = healthPointMax;
				}
			}
			else {
				healthPoint -= 30 * num;
				// todo: play effect?
				if (healthPoint < 1) {
					healthPoint = 1;
				}
			}
		}

		if (auto num = buffs.At(BuffTypes::肉食主义)) {
			buffs._肉食主义_damageScaleCount += 0.3f * num;
		}

	}

	bool Creature::OnDie() {
		if (auto num = buffs.At(BuffTypes::裁员大动脉)) {
			if (buffs._裁员大动脉_usedCount < num) {
				++buffs._裁员大动脉_usedCount;
				healthPoint = healthPointMax * std::powf(0.2f, (float)num);
				auto dmg = sp.luckyPoint * 3 * num;
				// todo: hit monsters
			}
		}
		return true;
	}

	XX_INLINE std::pair<Stat_t, bool> Creature::GetFinalDamageOnce(Stat_t skillDamage) {
		auto d = skillDamage * sp.damageRatio + buffs._肉食主义_damageScaleCount;
		if (stage->rnd.Next<float>() < sp.criticalChance) {		// calculate critical
			d += d * sp.criticalBonusRatio;
			return { d, true };
		}
		return { d, false };
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
