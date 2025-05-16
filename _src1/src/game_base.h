#pragma once

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
	struct EnvGrass;
	struct Loot;

	enum class State : uint32_t {
		Unknown = 0,
		Idle,
		Attack,
		ShootDelay,
		Knockback,
		DashDelay,
		Dashing,
		Hurt,
	};

	enum class CreatureTypes : uint32_t {
		Unknown = 0,
		Monster_Chip,
		Monster_Hamburger,
		Monster_Cola,
		Monster_House,
		Monster_RoastDuck,
		Monster_Laptop,
		Monster_Sofa,
		Monster_InstantNoodles,
		Player_Programmer,
		Player_Boss,
	};

	// stage's base
	struct Stage : xx::SceneBase {
		//xx::Camera camera;
		xx::CameraEx camera;
		xx::FrameBuffer fb;
		xx::Shared<xx::Node> ui;

		XYi gridSize{};							// grid's num cols rows
		XY mapSize{};							// cellSize * gridSize( grid pixel size )
		XY lastWindowSize{};					// for OnWindowSizeChanged

		xx::Rnd rnd;							// for game logic only
		int32_t time{};							// for game logic only
		int32_t roundId{};						// for round coroutine
		int32_t n{};							// for round coroutine
		int32_t sleepCounter{};					// for round coroutine

		xx::Listi32<xx::Shared<Bullet>> playerBullets;
		xx::Listi32<xx::Shared<Bullet>> monsterBullets;
		xx::Shared<Player> player;
		Space<Loot> loots;
		xx::Listi32<xx::Shared<Loot>> flyingLoots;
		Space<Monster> monsters;
		xx::Listi32<xx::Shared<Spawner>> spawners;
		xx::Shared<Ground> ground;
		xx::Listi32<xx::Shared<EnvGrass>> grasses;
		xx::Listi32<xx::Shared<MonsterGen>> monsterGenerators;
		xx::Listi32<xx::Shared<Drawable>> effects;
		EffectTextManager effectTexts;
		template<bool updateTime = true>
		void StageUpdate();
		template<bool clearPlayer, bool clearGrass>
		void ClearItems();						// for round finished
		// todo

		void StageInit(XYi gridSize_);
		virtual XY GetPlayerBornPos();
		void ForceLimit(XY& pos);
		bool IsOutOfMap(XY const& pos);
		XY GetRndPosDoughnut(float maxRadius, float safeRadius);
		void Update() override { StageUpdate(); };
		virtual void DrawCustomOrderYItem(xx::Listi32<std::pair<float, Game::Drawable*>>& yd, XY areaMin, XY areaMax) {};
		virtual void DrawCustomUI() {};
		virtual void OnWindowSizeChanged() {};
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

	// stage monster dead drop objects
	struct Loot : StageItem {
		CreatureTypes creatureType{};
		int32_t tagId{};
		Stat_t movementSpeedPerFrame{10};

		// ...
		void Init(Creature* creature_);
		void Collect(Creature* owner);
		int32_t Update() override;
		void Draw() override;
		void DrawFlying();
	};

	// stage creature's base
	struct Creature : StageItem {
		CreatureTypes creatureType{};
		int32_t tagId{};
		xx::Ref<xx::Frame> tagFrame;

		StatPanel sp;
		Stat_t healthPointMax{};
		Stat_t healthPoint{};				// left / current
		Stat_t damageReduce{};
		Stat_t dodgeChance{};
		Stat_t movementSpeed{};
		Stat_t movementSpeedPerFrame{};
		double coin{};

		BuffContainer buffs;
		StatCfg statCfg;

		void StatCalc();
		void OnRoundBegin();
		void OnRoundEnd();
		void OnKilled(Creature* tar);
		bool OnDie();		// return false: no die
		virtual void UpdateHealthPoint(int32_t hp) {};
		virtual void UpdateDamage(float damage) {}
		virtual void UpdateAttackSpeed(float attackSpeed) {}
		std::pair<Stat_t, bool> GetFinalDamageOnce(Stat_t skillDamage);	// second = true: critical

		xx::Listi32<xx::Shared<Skill>> skills;
		State state{};
		int32_t castingSkillCount{};

		bool knockback{};
		XY knockbackDist{};
		float knockbackSpeed{};
		float knockbackReduceValuePerFrame{};
		// todo

		int32_t n_Idle{};
		void Idle();				// coroutine
	};

	// player's base
	struct Player : Creature {
		static constexpr int32_t cProtectFrameCount{ int32_t(Cfg::fps * 1) };
		int32_t protectFrame{};
		Stat_t collectRange{};
		int32_t Hurt();
		// todo: pick up loot logic when update
	};

	// monster's base
	struct Monster : Creature {
		XY tarOffset{};
		float tarOffsetRadius{};
		// todo
		XY tagPos{};

		int32_t Hurt(float dmg, XY const& txtD, XY const& knockbackD, bool isCrit = false);		// return !0 mean dead
		void Knockback(float speed, XY const& d);
		int32_t Update() override;
		int32_t MoveToPosition(xx::XY targetPos, float targetRadius);
		int32_t MoveToPlayer(float keepDistance = 0);
		virtual void Rewards() {};		// drop items
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
