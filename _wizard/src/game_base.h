#pragma once

namespace Game {

	struct Scene : xx::SceneBase {
		xx::Shared<xx::Node> ui;
		void Update() override;
		void Draw() override;
		XY lastWindowSize{};					// for OnWindowSizeChanged
		virtual void OnWindowSizeChanged() {};
	};

	// predef
	struct Bullet;
	struct Player;
	struct Ground;
	struct Monster;
	struct Drawable;
	struct Creature;
	struct Stage;

	// stage's base
	struct Stage : xx::SceneBase {
		xx::FrameBuffer fb;
		xx::Shared<xx::Node> ui;

		XY lastWindowSize{};					// for OnWindowSizeChanged

		xx::Rnd rnd;							// for game logic only
		int32_t time{};							// for game logic only
		int32_t roundId{};						// for round coroutine
		int32_t n{};							// for round coroutine
		int32_t sleepCounter{};					// for round coroutine

		//xx::Listi32<xx::Shared<Bullet>> playerBullets;
		//xx::Listi32<xx::Shared<Bullet>> monsterBullets;
		//xx::Shared<Player> player;
		//Space<Loot> loots;
		//xx::Listi32<xx::Shared<Loot>> flyingLoots;
		//Space<Monster> monsters;
		//xx::Listi32<xx::Shared<Spawner>> spawners;
		//xx::Shared<Ground> ground;
		//xx::Listi32<xx::Shared<MonsterGen>> monsterGenerators;
		//xx::Listi32<xx::Shared<Drawable>> effects;
		//EffectTextManager effectTexts;
		//template<bool updateTime = true>
		//void StageUpdate();
		//template<bool clearPlayer, bool clearGrass>
		//void ClearItems();						// for round finished
		//// todo

		//template<bool initGround = true>
		//void StageInit(XYi gridSize_);
		//virtual XY GetPlayerBornPos();
		//void ForceLimit(XY& pos);
		//bool IsOutOfMap(XY const& pos);
		//XY GetRndPosDoughnut(float maxRadius, float safeRadius);
		//void Update() override { StageUpdate(); };
		//virtual void DrawCustomOrderYItem(xx::Listi32<std::pair<float, Game::Drawable*>>& yd, XY areaMin, XY areaMax) {};
		//virtual void DrawCustomUI() {};
		//virtual void OnWindowSizeChanged() {};
		//void Draw() override;
	};

	// animation: idle 's config( global usage )
	struct IdleCfg {
		static constexpr float cTotal{ 2.f };
		static constexpr float cRange{ 0.07f };
		static constexpr float cTimeSpan{ 0.3f };
		static constexpr float cInc{ cRange / (Cfg::fps * cTimeSpan) };
		static constexpr float cEnd{ 1.f - cRange };
	};

	// stage item's base
	struct alignas(8) Drawable {
		Stage* stage{};										// stage's life > this
		xx::Ref<xx::Frame> frame;

		xx::XY pos{};										// pivot position
		xx::XY scale{ 1.f, 1.f };							//
		xx::XY offsetRatio{ 0.5f, 0.3f};					// pos + size * scale * offsetRatio == center pos
		bool needFlipX{};
		float radius{}, radians{};							// for logic
		int32_t whiteColorEndTime{};						// for hurt effect
		int32_t destroyTime{};								// max life cycle for bug issue
		xx::RGBA8 color{xx::RGBA8_White};
		// todo

		virtual int32_t Update() { return 0; };				// return !0 mean need Release/Delete/Remove
		virtual void Draw();
		virtual ~Drawable() {};
	};

	// 
	struct StageItem : Drawable {
		int32_t indexAtItems{ -1 }, indexAtCells{ -1 };		// for space index
		StageItem* prev{}, * next{};						// for space index
	};

	// stage creature's base
	struct Creature : StageItem {
	};

	// player's base
	struct Player : Creature {
	};

	// monster's base
	struct Monster : Creature {
	};

	// monster generator's base
	struct MonsterGen {
		Stage* stage{};				// stage's life > this

		int32_t activeTime{}, destroyTime{};
		float countPool{}, countIncPerFrame{};

		void Init(Stage* stage_, int32_t activeTime_, int32_t destroyTime_, float generateNumsPerSeconds_);
		virtual int32_t Update() { return 0; }
		template<typename T> void CreateFromRect();
		template<typename T> void CreateFromDoughnut();
		template<typename T> void CreateFromFullMap();
	};

	// bullet's base
	struct Bullet : StageItem {
		xx::Weak<Creature> owner;										// owner's life maybe <= this

		float moveSpeed{};
		float radius{};
		float damage{};

		virtual int32_t Update() { return 0; }
	};

}
