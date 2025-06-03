#pragma once

namespace Game {

	struct Player;
	struct PlayerWeapon;
	struct PlayerBullet : Drawable {
		static constexpr float cMoveSpeed{ 500.f / Cfg::fps };
		xx::Weak<Player> owner;

		int32_t pierceCount{}, cPierceDelay{};
		xx::Listi32<std::pair<xx::Weak<Drawable>, int32_t>> pierceBlackList;	// second: timeout

		float radius{};
		float radians{};
		XY moveInc{};

		PlayerWeaponProperties2b pwp2b;

		void Init(PlayerWeapon* shooter, XY pos_, float radians_);
		int32_t Update() override;
		void Draw() override;
		void DrawLight() override;
	};

}
