#pragma once

namespace Game {

	struct Monster;
	struct MonsterBullet1_1 : MonsterBullet {
		void Init(Monster* shooter_, XY pos_, float radians_);
		int32_t Update() override;
		void Draw() override;
		void DrawLight(float colorPlus_) override;
		void PlayDeathEffect(float scale_) override;
	};

}
