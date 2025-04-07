﻿#pragma once
#include <looper.h>

namespace Game {

	enum class StatTypes : uint32_t {
		healthPoint,
		healthRegeneration,
		defensePoint,
		dodgePoint,
		movementSpeedPoint,
		damageRatio,
		criticalChance,
		criticalBonusRatio,
		attackSpeed,
		luckyPoint,
		__MAX_VALUE__
	};

	static constexpr std::array<const char*, (uint32_t)StatTypes::__MAX_VALUE__> strings_StatTypes{
		"healthPoint",
		"healthRegeneration",
		"defensePoint",
		"dodgePoint",
		"movementSpeedPoint",
		"damageRatio",
		"criticalChance",
		"criticalBonusRatio",
		"attackSpeed",
		"luckyPoint",
	};

	using Stat_t = float;
	struct alignas(8) StatItem {
		StatTypes type;
		Stat_t value;
	};

	struct alignas(8) StatPanel {
		static constexpr int32_t numFields{ (int32_t)StatTypes::__MAX_VALUE__ };
		Stat_t healthPoint;
		Stat_t healthRegeneration;
		Stat_t defensePoint;
		Stat_t dodgePoint;
		Stat_t movementSpeedPoint;
		Stat_t damageRatio;
		Stat_t criticalChance;
		Stat_t criticalBonusRatio;
		Stat_t attackSpeed;		// times per seconds
		Stat_t luckyPoint;
		void Clear() {
			memset(this, 0, sizeof(StatPanel));
		}
		constexpr Stat_t& operator[](int32_t idx) const {
			assert(idx <= numFields);
			return ((Stat_t*)this)[idx];
		}
		void Dump() {
			for (int32_t i = 0; i < numFields; ++i) {
				xx::CoutN(strings_StatTypes[i], " = ", operator[](i));
			}
		}
	};

	struct StatCfg {
		static constexpr float baseMovementSpeed{ 300 / 5 };
		static constexpr Stat_t dodgeFactor{ 16 };
		static constexpr Stat_t defenseFactor{ 66 };
		static constexpr StatPanel rangeFrom{};	// all zero
		static constexpr StatPanel rangeTo{
			.healthPoint = 999999,
			.healthRegeneration = 999999,
			.defensePoint = 999999,
			.dodgePoint = 65,
			.movementSpeedPoint = 25,
			.damageRatio = 999999,
			.criticalChance = 1,
			.criticalBonusRatio = 1.5,
			.attackSpeed = 999999,
			.luckyPoint = 999999,
		};
		StatPanel init{
			.healthPoint = 1000,
			.healthRegeneration = 5,
			.defensePoint = 0,
			.dodgePoint = 0,
			.movementSpeedPoint = 5,
			.damageRatio = 1,
			.criticalChance = 0.2,
			.criticalBonusRatio = 1.5,
			.attackSpeed = 1,
			.luckyPoint = 10,
		};
	};

	template<typename E>
	struct StatBase {
		xx::TinyList<xx::Shared<E>> equipments;
		StatCfg statCfg;
		StatPanel sp;
		bool spDirty{ true };
		Stat_t hp{};			// left / current
		Stat_t defenseRatio{};
		Stat_t dodgeRatio{};
		Stat_t movementSpeed{};
		Stat_t movementSpeedPerFrame{};

		// first, need assign statCfg.init.xxxxxxxxx = xxxxxxxxxxx
		void StatCalc() {
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
			// apply rules
			for (int32_t i = 0; i < StatPanel::numFields; ++i) {
				auto& v = sp[i];
				if (v < statCfg.rangeFrom[i]) v = statCfg.rangeFrom[i];
				else if (v > statCfg.rangeTo[i]) v = statCfg.rangeTo[i];
			}
			// fill some final result
			defenseRatio = sp.defensePoint / (sp.defensePoint + statCfg.defenseFactor);
			dodgeRatio = sp.dodgePoint / (sp.dodgePoint + statCfg.dodgeFactor);
			movementSpeed = statCfg.baseMovementSpeed * sp.movementSpeedPoint;
			movementSpeedPerFrame = movementSpeed * Cfg::frameDelay;
		}
	};
};
