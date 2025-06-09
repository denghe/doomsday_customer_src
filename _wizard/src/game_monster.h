#pragma once

namespace Game {

	struct Monster : Drawable {
		static constexpr xx::RGBA8 cLightColor{ 255,127,127,255 };	// red
		static constexpr float cLightRadiusRatio{ 1.f };
		static constexpr float cLightColorPlus{ 0.5f };
		static constexpr float cFlySpeed{ 500.f / Cfg::fps };
		static constexpr float cMoveSpeed{ 100.f / Cfg::fps };
		static constexpr float cTraceSpeed{ 200.f / Cfg::fps };
		static constexpr int32_t cShootDelay{ int32_t(1.f * Cfg::fps) };
		static constexpr XY cShootOffset{ 0.5f, 0 };	// ratio
		static constexpr int32_t cRayTracePlayerInterval{ int32_t( 0.2f * Cfg::fps ) };

		int32_t indexAtItems{ -1 }, indexAtCells{ -1 };
		Monster* prev{}, * next{};

		xx::Ref<xx::Frame> frame, lightFrame;
		int32_t flyTargetsIdx{};

		float radius{};
		float radians{};

		Shaker shaker;

		XY shootPos{};
		int32_t shootTime{};
		int32_t rayTracePlayerTime{};
		bool rayTracePlayerSuccess{};	// can hit to player

		int32_t _n{};	// for Update()

		MonsterProperties mp;

		template<bool withOffset>
		XY GetFlyTargetPos();
		bool MoveTo(XY tarPos, float speed);
		void MoveToPlayer(float speed);
		void HandleBlocks();
		void SyncPos();		// force limit & space index update
		void TracePlayer();
		void TryShoot();

		void Init(Stage* stage_, XY bornPos, int32_t flyTargetsIdx_);
		int32_t Update() override;
		void Draw() override;
		void DrawLight() override;

		void PlayDeathEffect(float scale_);
		// return actual hurt dp, int: 0 normal 1 dodge 2 death( play death effect )
		std::pair<float, int> Hurt(float dp);
	};

}
