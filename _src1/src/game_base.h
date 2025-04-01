#pragma once

namespace Game {

	/*
	
	main relationships:

	stage
		L-- players
		L-- player's bullets
		L-- monsters
		L-- monster's bullets
		L-- map / blocks / envs / effects
		L-- master logic
			L-- skill configs( drop ways & rates & configs )
			L-- monster generators ( timeline )

	player : creature
	monster : creature
	creature
		L--  skills

	skill ( bullet generator )
		L-- skill config

	bullet
		L-- skill config ( copy from skill )
	
	*/

	// predef
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
		xx::Listi32<xx::Shared<SkillCfg>> skillCfgs;
		// todo

		virtual XY GetPlayerBornPos();
		void ForceLimit(XY& pos);
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
	struct Drawable {
		Stage* stage{};				// stage's life > this
		xx::XY pos{}, scale{};
		float radius{}, radians{};
		// todo
		virtual void Draw() {};
		virtual ~Drawable() {};
	};

	// stage creature's base
	struct Creature : Drawable {
		float moveSpeed{};
		float radius{};
		float damage{};
		float criticalRate{};
		float criticalDamageRatio{};
		// todo

		xx::Listi32<xx::Shared<Skill>> skills;

		int32_t idle_lineNumber{};
		void Idle();				// coroutine

		virtual int32_t Update() { return 0; }
	};

	// player's base
	struct Player : Creature {
		// todo
	};

	// monster's base
	struct Monster : Creature {
		int32_t indexAtItems{ -1 }, indexAtCells{ -1 };		// for space index
		Monster* prev{}, * next{};							// for space index
		// todo
	};

	// monster generator's base
	struct MonsterGen {
		Stage* stage{};				// stage's life > this

		int32_t activeTime{}, destroyTime{};
		float countPool{}, countIncPerFrame{};

		void Init(Stage* stage_, int32_t activeTime_, int32_t destroyTime_, float generateNumsPerSeconds_);
		virtual void Update() {};
	};

	// bullet's base
	struct Bullet : Drawable {
		xx::Weak<Creature> ownere;	// owner's life maybe <= this
		SkillCfg* cfg;				// skill cfg's life > this( copy from maker )

		virtual int32_t Update() { return 0; }
	};

	struct PlayerBullet : Bullet {
		// todo
	};

	struct MonsterBullet : Bullet {
		int32_t indexAtItems{ -1 }, indexAtCells{ -1 };		// for space index
		Monster* prev{}, * next{};							// for space index
		// todo
	};

	// player's skill's base
	struct Skill {
		Player* player{};			// player's life > this
		SkillCfg* cfg;				// skill cfg's life > this
		float shootCountPool{};		// time pool for shoot

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
