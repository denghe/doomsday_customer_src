#pragma once

namespace Game {

	struct Player;
	struct PlayerBullet : Drawable {
		static constexpr XY cShootOffset{ ResTpFrames::_size_char_weapon.x * 0.5f, 0 };
		xx::Weak<Player> owner;
		float radians{};
		XY GetShootPos();
		void Init(Player* owner_);
		int32_t Update() override;
		void Draw() override;
	};

}
