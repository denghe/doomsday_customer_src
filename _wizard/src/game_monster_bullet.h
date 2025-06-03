#pragma once

namespace Game {

	struct Monster;
	struct MonsterBullet : Drawable {
		static constexpr float cMoveSpeed{ 100.f / Cfg::fps };

		int32_t indexAtItems{ -1 }, indexAtCells{ -1 };
		MonsterBullet* prev{}, * next{};

		float radius{};
		float radians{};
		XY moveInc{};

		MonsterProperties2 mp2;

		void Init(Monster* shooter_, XY pos_, float radians_);
		int32_t Update() override;
		void Draw() override;
		void DrawLight() override;
	};

}
