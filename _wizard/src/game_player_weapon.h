#pragma once

namespace Game {

	struct Player;
	struct PlayerWeapon : Drawable {
		static constexpr float cAnchorX{ 0.3f };
		static constexpr XY cShootOffset{ ResTpFrames::_size_char_weapon.x * (1 - cAnchorX), 0 };
		xx::Weak<Player> owner;
		XY holdOffset{};
		XY shootPos{};
		float radians{}, cos{}, sin{};
		int32_t nextShootTime{};
		PlayerWeaponProperties pwp;
		void Init(Player* owner_, XY holdOffset_);
		void UpdatePosRadians();
		int32_t Update() override;
		void Draw() override;
		void DrawLight(float colorPlus_) override;
	};


	struct PlayerWeapon_1 : PlayerWeapon {
		static constexpr float cMaxDistance{ Cfg::unitSize * 35.f };
		static constexpr float cRayTraceBoxRadius{ Cfg::unitSize * 0.1f };
		XY tarPos{};
		float uvOffset{}, distance{};
		int32_t shooting{};
		xx::Listi32<std::pair<xx::Weak<Drawable>, int32_t>> hitBlackList;	// second: timeout

		int32_t Update() override;
		void Draw() override;
		void DrawLight(float colorPlus_) override;
		void PlayDeathEffect(XY pos_, float scale_);

	};
}
