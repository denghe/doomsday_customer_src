#pragma once

namespace Game {

	struct Monster : Drawable {
		static constexpr xx::RGBA8 cLightColor{ 255,127,127,255 };	// red
		static constexpr float cLightRadiusRatio{ 1.f };
		static constexpr float cLightColorPlus{ 0.5f };
		static constexpr float cFlySpeed{ 500.f / Cfg::fps };
		static constexpr float cMoveSpeed{ 100.f / Cfg::fps };
		static constexpr int32_t cShootDelay{ int32_t(1.f * Cfg::fps) };
		static constexpr XY cShootOffset{ 0.5f, 0 };	// ratio
		static constexpr int32_t cRayTracePlayerInterval{ int32_t( 0.5f * Cfg::fps ) };

		int32_t indexAtItems{ -1 }, indexAtCells{ -1 };
		Monster* prev{}, * next{};

		xx::Ref<xx::Frame> frame, lightFrame;
		xx::Weak<MonsterFormation> monsterFormation;
		int32_t monsterFormationPosIdx{};

		float radius{};
		float radians{};

		XY shootPos{};
		int32_t shootTime{};
		int32_t rayTracePlayerTime{};
		bool rayTracePlayerSuccess{};	// can hit to player

		int32_t _n{};

		MonsterProperties mp;

		bool MoveToTarget(float speed);
		void HandleBlock();
		void SyncPos();
		void FaceToPlayer();
		void TryShoot();

		void Init(Stage* stage_, int32_t bornPosIdx, xx::Weak<MonsterFormation> monsterFormation_, int32_t monsterFormationPosIdx_);
		int32_t Update() override;
		void Draw() override;
		void DrawLight() override;

		void PlayDeathEffect(float scale_);
		// return actual hurt dp, int: 0 normal 1 dodge 2 death( play death effect )
		std::pair<float, int> Hurt(float dp);
	};

}
