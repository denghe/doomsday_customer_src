#pragma once

namespace Game {

	struct Player;
	struct PlayerWeapon;
	struct PlayerBullet : Drawable {
		xx::Weak<Player> owner;
		PlayerWeaponProperties2b pwp2b;
	};

}
