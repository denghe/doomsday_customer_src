#pragma once

namespace Game {

	struct PlayerBullet_1 : Bullet {
		XY inc{};							// move increase pixel per frame
		int32_t lifeEndTime{};				// max life point

		void Init(ShootSkill* skill, XY const& pos_, float radians_, float cos, float sin);
		int32_t Update() override;
		void Draw() override;
	};

	// ...

}
