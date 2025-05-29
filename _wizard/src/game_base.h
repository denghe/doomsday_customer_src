#pragma once

namespace Game {

	// predef
	struct Bullet;
	struct Player;
	struct Map;
	struct Monster;
	struct Drawable;
	struct Creature;

	// stage's base
	struct Stage : Scene {
		xx::Rnd rnd;							// for game logic only
		int32_t time{};							// for game logic only

		//xx::Listi32<xx::Shared<Bullet>> playerBullets;
		//xx::Listi32<xx::Shared<Bullet>> monsterBullets;
		//xx::Shared<Player> player;
		//Space<Loot> loots;
		//xx::Listi32<xx::Shared<Loot>> flyingLoots;
		//Space<Monster> monsters;
		//xx::Listi32<xx::Shared<Spawner>> spawners;
		//xx::Shared<Map> ground;
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

	// bullet's base
	struct Bullet : StageItem {
		xx::Weak<Creature> owner;										// owner's life maybe <= this

		float moveSpeed{};
		float radius{};
		float damage{};

		virtual int32_t Update() { return 0; }
	};

}
