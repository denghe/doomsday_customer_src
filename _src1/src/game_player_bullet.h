#pragma once

namespace Game {

	struct Player;

	struct PlayerBullet : Drawable {
		xx::Weak<Player> ownerPlayer;
		PlayerSkillCfg cfg;
		float radians{};

		virtual int32_t Update() { return 0; }	// need override
	};

	struct PlayerBullet_1 : PlayerBullet {
		// todo: more args
		void Init(PlayerSkill* ps);
		int32_t Update() override;
		void Draw() override;
	};

	// ...

}
