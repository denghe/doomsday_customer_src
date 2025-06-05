#pragma once

namespace Game {

	struct Player;
	struct PlayerWeapon : Drawable {
		static constexpr float cAnchorX{ 0.3f };
		static constexpr XY cShootOffset{ ResTpFrames::_size_char_weapon.x * (1 - cAnchorX), 0 };
		xx::Weak<Player> owner;
		XY holdOffset{};
		float radians{};
		int32_t nextShootTime{};
		PlayerWeaponProperties pwp;
		XY GetShootPos();
		void Init(Player* owner_, XY holdOffset_);
		int32_t Update() override;
		void Draw() override;
		void DrawLight() override;
	};

}
