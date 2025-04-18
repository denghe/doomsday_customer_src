#pragma once

namespace Game {

	struct Bullet_2 : Bullet {
		XY inc{};							// move increase pixel per frame
		int32_t lifeEndTime{};				// max life point

		void Init(Skill_Shoot_1* skill, XY const& pos_, float radians_, float lifeSeconds_, float cos, float sin);
		int32_t Update() override;
		void Draw() override;
	};

	// ...

}
