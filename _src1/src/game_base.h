﻿#pragma once

namespace Game {

	/*
	
	main relationships:

	stage
		L-- players
		L-- player's bullets
		L-- monsters
		L-- monster's bullets
		L-- map / blocks / envs / effects / spawners
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
	struct Drawable;
	struct Spawner;
	struct Creature;

	enum class State : uint32_t {
		Unknown = 0,
		Idle,
		Attack,
		Knockback,
		PreDash,
		Dashing,
		Hurt,
	};

	// stage's base
	struct Stage : xx::SceneBase {
		//xx::Camera camera;
		xx::CameraEx camera;
		xx::Shared<xx::Node> ui;

		XYi gridSize{};		// grid's num cols rows
		XY mapSize{};		// cellSize * gridSize( grid pixel size )

		bool paused{};		// for pop ui stop logic
		int32_t time{};		// frame number
		int32_t round{};	// round number
		xx::Rnd rnd;

		xx::Listi32<xx::Shared<Bullet>> playerBullets;
		xx::Listi32<xx::Shared<Bullet>> monsterBullets;
		xx::Shared<Player> player;
		Space<Monster> monsters;
		xx::Listi32<xx::Shared<Spawner>> spawners;
		xx::Shared<Ground> ground;
		xx::Listi32<xx::Shared<MonsterGen>> monsterGenerators;
		xx::Listi32<xx::Shared<Drawable>> effects;
		EffectTextManager etm;
		//xx::Listi32<xx::Shared<SkillCfg>> skillCfgs;
		// todo

		std::function<void()> onCleanup;

		virtual XY GetPlayerBornPos();
		void ForceLimit(XY& pos);
		bool IsOutOfMap(XY const& pos);
		XY GetRndPosDoughnut(float maxRadius, float safeRadius);
		void Update() override;
		virtual void DrawCustomUI() {};
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

		std::u32string name;								// for debug?
		xx::Listi32<xx::TinyFrame> nameFrames;				// store name -> frame list
		XY nameSize{}, namePosOffset{};						// anchor == 0.5
		void SetName(XY const& posOffset, std::u32string_view const& name_);		// name_ -> name
		void SetName(XY const& posOffset, std::string_view const& name_);
		void FillNameFrames(std::u32string_view const& name_);
		void DrawNameBG();
		void DrawName();
	};

	// creature's equipments base
	struct EquipmentBase {
		xx::Weak<Creature> owner;
		xx::TinyList<StatItem> stats;		// maybe use fixed length array instead of list
		// todo
	};

	// stage creature's base
	struct Creature : StageItem {

		StatPanel sp;
		Stat_t healthPoint{};				// left / current
		Stat_t defenseRatio{};
		Stat_t dodgeChance{};
		Stat_t movementSpeed{};
		Stat_t movementSpeedPerFrame{};
		double coin{};

		xx::TinyList<xx::Shared<EquipmentBase>> equipments;
		StatCfg statCfg;

		void StatCalc();

		xx::Listi32<xx::Shared<Skill>> skills;
		State state{};
		int32_t castingSkillCount{};

		bool knockback{};
		XY knockbackDist{};
		float knockbackSpeed{};
		float knockbackReduceValuePerFrame{};

		// todo
		// todo: inventory ? buff collection?

		int32_t n_Idle{};
		void Idle();				// coroutine
	};

	// player's base
	struct Player : Creature {
		// todo
	};

	// monster's base
	struct Monster : Creature {
		XY tarOffset{};
		float tarOffsetRadius{};
		// todo

		int32_t Hurt(float dmg, XY const& txtD, XY const& knockbackD, bool isCrit = false);		// return !0 mean dead
		void Knockback(float speed, XY const& d);
		int32_t Update() override;
		int32_t MoveToPosition(xx::XY targetPos, float targetRadius);
		int32_t MoveToPlayer();
	};

	// monster generator's base
	struct MonsterGen {
		Stage* stage{};				// stage's life > this
		StatCfg statCfg;

		int32_t activeTime{}, destroyTime{};
		float countPool{}, countIncPerFrame{};

		StatPanel& Init(Stage* stage_, int32_t activeTime_, int32_t destroyTime_, float generateNumsPerSeconds_);
		virtual int32_t Update() { return 0; }
		template<typename T> void CreateFromRect();
		template<typename T> void CreateFromDoughnut();
		template<typename T> void CreateFromFullMap();
	};

	// bullet's base
	struct Bullet : StageItem {
		xx::Weak<Creature> owner;										// owner's life maybe <= this

		// following fields: init by maker
		float moveSpeed{};
		float radius{};
		float damage{};
		Stat_t damageRatio{};
		Stat_t criticalChance{};
		Stat_t criticalBonusRatio{};

		virtual int32_t Update() { return 0; }
	};

	// creature's skill's base
	struct Skill {
		Creature* creature{};		// creature's life > this

		virtual int32_t Update() { return 0; };
		virtual ~Skill() {};
	};

}
