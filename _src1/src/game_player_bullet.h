#pragma once

namespace Game {

	struct PlayerBullet_1 : Bullet {
		// todo: more args
		void Init(Skill* ps);
		int32_t Update() override;
		void Draw() override;
	};

	// ...

}
