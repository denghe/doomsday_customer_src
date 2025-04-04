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
	struct DrawableEx;

	// stage's base
	struct Stage : xx::SceneBase {
		xx::Camera camera;
		xx::Shared<xx::Node> ui;

		XYi gridSize{};		// grid's num cols rows
		XY mapSize{};		// cellSize * gridSize( grid pixel size )

		int32_t time{};		// frame number
		int32_t round{};	// round number
		xx::Rnd rnd;

		xx::Listi32<xx::Shared<Bullet>> playerBullets;
		xx::Shared<Player> player;
		Space<Monster> monsters;
		xx::Shared<Ground> ground;
		xx::Listi32<xx::Shared<MonsterGen>> monsterGenerators;
		xx::Listi32<xx::Shared<DrawableEx>> effects;
		EffectTextManager etm;
		xx::Listi32<xx::Shared<SkillCfg>> skillCfgs;
		// todo

		std::function<void()> onCleanup;

		virtual XY GetPlayerBornPos();
		void ForceLimit(XY& pos);
		bool IsOutOfMap(XY const& pos);
		XY GetRndPosDoughnut(float maxRadius, float safeRadius);
		void Update() override;
		void Draw() override;
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
		Stage* stage{};										// stage's life > this
		xx::Ref<xx::Frame> frame;
		xx::XY pos{}, scale{};
		bool needFlipX{};
		float radius{}, radians{};							// for logic
		int32_t whiteColorEndTime{};						// for hurt effect
		int32_t destroyTime{};								// max life cycle for bug issue
		// todo
		virtual void Draw();
		virtual ~Drawable() {};
	};
	struct DrawableEx : Drawable {
		using Drawable::Drawable;
		virtual int32_t Update() { return 0; };				// return !0 mean need Release/Delete/Remove
	};

	// stage creature's base
	struct Creature : DrawableEx {
		xx::Listi32<xx::Shared<Skill>> skills;
		float hp{};
		float damage{};
		float moveSpeed{};
		float criticalRate{};
		float criticalDamageRatio{};

		bool knockback{};
		XY knockbackDist{};
		float knockbackSpeed{};
		float knockbackReduceValuePerFrame{};
		// todo

		int32_t idle_lineNumber{};
		void Idle();				// coroutine
	};

	// player's base
	struct Player : Creature {
		// todo
	};

	// monster's base
	struct Monster : Creature {
		int32_t indexAtItems{ -1 }, indexAtCells{ -1 };		// for space index
		Monster* prev{}, * next{};							// for space index

		XY tarOffset{};
		float tarOffsetRadius{};
		// todo

		int32_t Hurt(float dmg, XY const& txtD, XY const& knockbackD);		// return !0 mean dead
		void Knowckback(float speed, XY const& d);
		int32_t Update() override;
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
		xx::Weak<Creature> owner;	// owner's life maybe <= this
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
		int32_t typeId{};			// for switch case cast Derived*
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
