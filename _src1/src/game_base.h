#pragma once

namespace Game {

	/*
	
	main relationships:

	stage
		L-- players
		L-- player's bullets
		L-- monsters
		L-- monster's bullets
		L-- master logic
			L-- skill configs( drop ways & rates & configs )
			L-- monster generators ( timeline )

	player
		L-- skills

	monster
		L-- skills

	skill ( bullet generator )
		L-- skill config

	bullet
		L-- skill config ( copy from skill )
	
	*/

	// predefine base classes
	struct Bullet;
	struct Player;
	struct Ground;
	struct Monster;
	struct MonsterGen;
	struct Skill;
	struct SkillCfg;

	// stage's base
	struct Stage : xx::SceneBase {
		xx::Camera camera;
		xx::Shared<xx::Node> ui;

		XYi gridSize{};		// grid's num cols rows
		XY mapSize{};		// cellSize * gridSize( grid pixel size )

		int32_t time{};		// frame number
		xx::Rnd rnd;

		xx::Listi32<xx::Shared<Bullet>> playerBullets;
		xx::Shared<Player> player;
		Space<Monster> monsters;
		xx::Shared<Ground> ground;
		xx::Listi32<xx::Shared<MonsterGen>> monsterGenerators;
		// todo: effects

		virtual XY GetPlayerBornPos();
		void ForceLimit(XY& pos);
	};

	// stage item's base
	struct Drawable {
		Stage* ownerStage{};	// stage's life > this. this is stage's child
		xx::XY pos{}, scale{};
		virtual void Draw() {};
		virtual ~Drawable() {};
	};

	// animation: idle 's config( global usage )
	struct IdleCfg {
		static constexpr float cTotal{ 2.f };
		static constexpr float cRange{ 0.07f };
		static constexpr float cTimeSpan{ 0.3f };
		static constexpr float cInc{ cRange / (Cfg::fps * cTimeSpan) };
		static constexpr float cEnd{ 1.f - cRange };
	};

	// player's base
	struct Player : Drawable {
		float moveSpeed{};
		float radius{};
		float damage{};
		float criticalRate{};
		float criticalDamageRatio{};
		xx::Listi32<xx::Shared<Skill>> skills;

		int32_t idle_lineNumber{};
		void Idle();		// coroutine

		virtual int32_t Update() { return 0; }
	};

	// monster's base
	struct Monster : Drawable {
		XY pos{};
		float radius{};
		int32_t indexAtItems{ -1 }, indexAtCells{ -1 };
		Monster* prev{}, * next{};

		virtual int32_t Update() { return 0; };
	};

	// monster generator's base
	struct MonsterGen {
		Stage* ownerStage{};	// stage's life > this

		int activeTime{}, destroyTime{};
		float countPool{}, countIncPerFrame{};

		void Init(Stage* ownerStage_, int activeTime_, int destroyTime_, float generateNumsPerSeconds_);
		virtual void Update() {};
	};

	// player's bullet's base
	struct Bullet : Drawable {
		xx::Weak<Player> ownerPlayer;	// player's life maybe <= this
		xx::Shared<SkillCfg> cfg{};
		float radians{};

		virtual int32_t Update() { return 0; }
	};

	// player's skill's base
	struct Skill {
		Player* ownerPlayer{};		// player's life > this

		xx::Shared<SkillCfg> cfg;
		float shootCountPool{};	// runtime

		virtual int32_t Update() { return 0; };
		virtual ~Skill() {};
	};

	// player's skill's config's base
	struct SkillCfg {
		float aimRange{};           // cell size * 10 ?
		float radius{};             // == res.size.x
		int32_t damage{};
		float moveSpeed{};          // ??? / fps
		float shootSpeed{};         // times per seconds / fps
		int32_t life{};             // seconds * fps
		int32_t pierceCount{};
		int32_t pierceDelay{};      // seconds * fps
	};

}
