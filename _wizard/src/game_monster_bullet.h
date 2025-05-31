#pragma once

namespace Game {

	struct Monster;
	struct MonsterBullet : Drawable {
		static constexpr xx::RGBA8 cLightColor{ 255,127,127,255 };	// red
		static constexpr float cLightRadiusRatio{ 1.f };
		static constexpr float cLightColorPlus{ 0.5f };
		static constexpr float cMoveSpeed{ 500.f / Cfg::fps };

		int32_t indexAtItems{ -1 }, indexAtCells{ -1 };
		MonsterBullet* prev{}, * next{};

		float radius{};
		float radians{};
		XY moveInc{};

		xx::RGBA8 lightColor{};
		float lightColorPlus{};
		XY lightRadius{};

		void Init(Monster* shooter_, XY pos_, float radians_);
		int32_t Update() override;
		void Draw() override;
	};

}
