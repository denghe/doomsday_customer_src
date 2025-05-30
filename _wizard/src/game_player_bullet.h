#pragma once

namespace Game {

	struct Player;
	struct PlayerWeapon;
	struct PlayerBullet : Drawable {
		static constexpr float cMoveSpeed{ 500.f / Cfg::fps };
		xx::Weak<Player> owner;

		float radius{};
		float radians{};
		XY moveInc{};

		xx::RGBA8 lightColor{};
		XY lightRadius{};

		void Init(PlayerWeapon* shooter, XY pos_, float radians_);
		int32_t Update() override;
		void Draw() override;
	};

}
