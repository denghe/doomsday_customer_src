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


	enum class StatTypes : uint32_t {
		healthPoint,
		healthRegeneration,
		defensePoint,
		dodgePoint,
		movementSpeedPoint,
		damageRatio,
		criticalChance,
		criticalBonusRatio,
		attackSpeed,
		luckyPoint,
		harvestRatio,
		__MAX_VALUE__
	};

	static constexpr std::array<const char*, (uint32_t)StatTypes::__MAX_VALUE__> strings_StatTypes{
		"healthPoint",
		"healthRegeneration",
		"defensePoint",
		"dodgePoint",
		"movementSpeedPoint",
		"damageRatio",
		"criticalChance",
		"criticalBonusRatio",
		"attackSpeed",
		"luckyPoint",
		"harvestRatio",
	};

	using Stat_t = float;
	struct alignas(8) StatItem {
		StatTypes type;
		Stat_t value;
	};

	struct alignas(8) StatPanel {
		static constexpr int32_t numFields{ (int32_t)StatTypes::__MAX_VALUE__ };
		Stat_t healthPoint;
		Stat_t healthRegeneration;
		Stat_t defensePoint;
		Stat_t dodgePoint;
		Stat_t movementSpeedPoint;
		Stat_t damageRatio;
		Stat_t criticalChance;
		Stat_t criticalBonusRatio;
		Stat_t attackSpeed;		// times per seconds
		Stat_t luckyPoint;
		Stat_t harvestRatio;
		void Clear() {
			memset(this, 0, sizeof(StatPanel));
		}
		constexpr Stat_t& operator[](int32_t idx) const {
			assert(idx <= numFields);
			return ((Stat_t*)this)[idx];
		}
		void Dump() {
			for (int32_t i = 0; i < numFields; ++i) {
				xx::CoutN(strings_StatTypes[i], " = ", operator[](i));
			}
		}
	};

	struct StatCfg {
		static constexpr Stat_t baseMovementSpeed{ 300 / 5 };
		static constexpr Stat_t dodgeFactor{ 16 };
		static constexpr Stat_t defenseFactor{ 66 };
		static constexpr Stat_t luckyToHealthRegenerationRatio{ 0.01 };
		static constexpr Stat_t luckyToDamageRatio{ 0.01 };
		static constexpr Stat_t luckyTocriticalChanceRatio{ 0.001 };

		static constexpr StatPanel rangeFrom{	// min val
			.healthPoint = 1,
			.healthRegeneration = 0,
			.defensePoint = 0,
			.dodgePoint = 0,
			.movementSpeedPoint = 0,
			.damageRatio = 0,
			.criticalChance = 0,
			.criticalBonusRatio = 0,
			.attackSpeed = 0,
			.luckyPoint = 0,
			.harvestRatio = 0,
		};
		static constexpr StatPanel rangeTo{		// max val
			.healthPoint = 999999,
			.healthRegeneration = 999999,
			.defensePoint = 999999,
			.dodgePoint = 65,
			.movementSpeedPoint = 25,
			.damageRatio = 999999,
			.criticalChance = 1,
			.criticalBonusRatio = 1.5,
			.attackSpeed = 999999,
			.luckyPoint = 1000,
			.harvestRatio = 999999,
		};
		StatPanel init{							// init val
			.healthPoint = 1000,
			.healthRegeneration = 5,
			.defensePoint = 0,
			.dodgePoint = 0,
			.movementSpeedPoint = 5,
			.damageRatio = 1,
			.criticalChance = 0.2,
			.criticalBonusRatio = 1.5,
			.attackSpeed = 1,
			.luckyPoint = 10,
			.harvestRatio = 0,
		};
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
