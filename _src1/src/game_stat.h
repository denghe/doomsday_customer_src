﻿#pragma once

namespace Game {

	enum class StatTypes : uint32_t {
		healthPoint,
		healthRatio,
		healthRegeneration,
		defensePoint,
		dodgePoint,
		dodgeRatio,
		movementSpeedPoint,
		damageRatio,
		criticalChance,
		criticalBonusRatio,
		attackSpeed,
		attackSpeedRatio,
		luckyPoint,
		harvestRatio,
		cameraScale,
		__MAX_VALUE__
	};

	static constexpr std::array<const char*, (uint32_t)StatTypes::__MAX_VALUE__> strings_StatTypes{
		"healthPoint",
		"healthRatio",
		"healthRegeneration",
		"defensePoint",
		"dodgePoint",
		"dodgeRatio",
		"movementSpeedPoint",
		"damageRatio",
		"criticalChance",
		"criticalBonusRatio",
		"attackSpeed",
		"attackSpeedRatio",
		"luckyPoint",
		"harvestRatio",
		"cameraZoom",
	};

	using Stat_t = float;
	struct alignas(8) StatItem {
		StatTypes type;
		Stat_t value;
	};

	struct alignas(8) StatPanel {
		static constexpr int32_t numFields{ (int32_t)StatTypes::__MAX_VALUE__ };
		Stat_t healthPoint;
		Stat_t healthRatio;
		Stat_t healthRegeneration;
		Stat_t defensePoint;
		Stat_t dodgePoint;
		Stat_t dodgeRatio;
		Stat_t movementSpeedPoint;
		Stat_t damageRatio;
		Stat_t criticalChance;
		Stat_t criticalBonusRatio;
		Stat_t attackSpeed;		// times per seconds
		Stat_t attackSpeedRatio;
		Stat_t luckyPoint;
		Stat_t harvestRatio;
		Stat_t cameraZoom;
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
		static constexpr Stat_t baseMovementSpeed{ 300 / 5 };
		static constexpr Stat_t dodgeFactor{ 16 };
		static constexpr Stat_t defenseFactor{ 66 };
		static constexpr Stat_t luckyToHealthRegenerationRatio{ 0.01 };
		static constexpr Stat_t luckyToDamageRatio{ 0.01 };
		static constexpr Stat_t luckyTocriticalChanceRatio{ 0.001 };

		static constexpr StatPanel rangeFrom{	// min val
			.healthPoint = 1,
			.healthRatio = 1,
			.healthRegeneration = 0,
			.defensePoint = 0,
			.dodgePoint = 0,
			.dodgeRatio = 0,
			.movementSpeedPoint = 0,
			.damageRatio = 0,
			.criticalChance = 0,
			.criticalBonusRatio = 0,
			.attackSpeed = 0,
			.attackSpeedRatio = 1,
			.luckyPoint = 0,
			.harvestRatio = 0,
			.cameraZoom = 1,
		};
		static constexpr StatPanel rangeTo{		// max val
			.healthPoint = 999999,
			.healthRatio = 999999,
			.healthRegeneration = 999999,
			.defensePoint = 999999,
			.dodgePoint = 65,
			.dodgeRatio = 1,
			.movementSpeedPoint = 25,
			.damageRatio = 999999,
			.criticalChance = 1,
			.criticalBonusRatio = 1.5,
			.attackSpeed = 999999,
			.attackSpeedRatio = 999999,
			.luckyPoint = 1000,
			.harvestRatio = 999999,
			.cameraZoom = 10,
		};
		StatPanel init{							// init val
			.healthPoint = 1000,
			.healthRatio = 1,
			.healthRegeneration = 5,
			.defensePoint = 0,
			.dodgePoint = 0,
			.dodgeRatio = 0,
			.movementSpeedPoint = 5,
			.damageRatio = 1,
			.criticalChance = 0.2,
			.criticalBonusRatio = 1.5,
			.attackSpeed = 1,
			.attackSpeedRatio = 1,
			.luckyPoint = 10,
			.harvestRatio = 0,
			.cameraZoom = 1,
		};
	};

}
