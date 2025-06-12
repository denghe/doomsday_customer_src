#pragma once

namespace Game {

	struct Monster;
	struct MonsterBullet : Drawable {
		static constexpr xx::RGBA8 cLightColor{ 0x63,0x9b,0xff,0xff };
		static constexpr float cMoveSpeed{ 100.f / Cfg::fps };

		int32_t indexAtItems{ -1 }, indexAtCells{ -1 };
		MonsterBullet* prev{}, * next{};

		float frameIndex{};
		float radius{};
		float radians{};
		XY moveInc{};

		MonsterProperties2 mp2;

		void Init(Monster* shooter_, XY pos_, float radians_);
		int32_t Update() override;
		void Draw() override;
		void DrawLight(float colorPlus_) override;

		void PlayDeathEffect(float scale_);
		// return actual hurt dp, int: 0 normal 1 dodge 2 death( play death effect + released )
		std::pair<float, int> Hurt(float dp);
	};

}
