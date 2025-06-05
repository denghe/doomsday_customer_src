#pragma once

namespace Game {

	// base & factor

	// for shooter
	struct PlayerWeaponProperties1a {
		float projectileAmountPreset{};
		float projectileAmountFactor{};
		float shootSpeedPreset{};
		float shootSpeedFactor{};
		float manaCostPreset{};
		float manaCostFactor{};
		float spreadPreset{};
		float spreadFactor{};
	};
	// for projectile
	struct PlayerWeaponProperties1b {
		float damagePointPreset{};
		float damagePointFactor{};
		float hurtDelaySecondsPreset{};
		float hurtDelaySecondsFactor{};
		float movementSpeedPreset{};
		float movementSpeedFactor{};
		float scalePreset{};
		float scaleFactor{};
		float knockbackPreset{};
		float knockbackFactor{};
		float durationPreset{};
		float durationFactor{};
		float effectRatePreset{};
		float effectRateFactor{};
		float effectPowerPreset{};
		float effectPowerFactor{};
	};

	// result & runtime

	// for shooter
	struct PlayerWeaponProperties2a {
		float projectileAmount{};
		float shootSpeed{};
		float manaCost{};
		float spread{};
	};

	// for projectile
	struct PlayerWeaponProperties2b {
		float damagePoint{};
		float hurtDelaySeconds{};
		float movementSpeed{};
		float scale{};
		float knockback{};
		float duration{};
		float effectRate{};
		float effectPower{};
	};

	struct PlayerWeaponProperties : PlayerWeaponProperties1a, PlayerWeaponProperties1b
	, PlayerWeaponProperties2a, PlayerWeaponProperties2b {

		// call at player init
		void Init() {
			projectileAmountPreset = 1.f;
			projectileAmountFactor = 1.f;
			shootSpeedPreset = 2.f;
			shootSpeedFactor = 1.f;
			manaCostPreset = 10.f;
			manaCostFactor = 1.f;
			spreadPreset = 1.f;
			spreadFactor = 1.f;
			damagePointPreset = 10.f;
			damagePointFactor = 1.f;
			hurtDelaySecondsPreset = 0.1f;
			hurtDelaySecondsFactor = 1.f;
			movementSpeedPreset = 300.f;
			movementSpeedFactor = 1.f;
			scalePreset = 1.f;
			scaleFactor = 1.f;
			knockbackPreset = 32.f;
			knockbackFactor = 1.f;
			durationPreset = 5.f;
			durationFactor = 1.f;
			effectRatePreset = 0.2f;
			effectRateFactor = 1.f;
			effectPowerPreset = 10.f;
			effectPowerFactor = 1.f;
		}

		// call at player init & equipment changed
		void CalcAll() {
			projectileAmount = projectileAmountPreset * projectileAmountFactor;
			shootSpeed = shootSpeedPreset * shootSpeedFactor;
			manaCost = manaCostPreset * manaCostFactor;
			spread = spreadPreset * spreadFactor;
			damagePoint = damagePointPreset * damagePointFactor;
			hurtDelaySeconds = hurtDelaySecondsPreset * hurtDelaySecondsFactor;
			movementSpeed = movementSpeedPreset * movementSpeedFactor;
			scale = scalePreset * scaleFactor;
			knockback = knockbackPreset * knockbackFactor;
			duration = durationPreset * durationFactor;
			effectRate = effectRatePreset * effectRateFactor;
			effectPower = effectPowerPreset * effectPowerFactor;
		}
	};

}
