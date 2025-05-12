#pragma once
#include <looper.h>

// unused

namespace PlatformGame {

	struct Scene;
	struct SceneItem {
		Scene* scene{};
		XY pos{}, size{};
		virtual int32_t Update() { return 0; };
		virtual void Draw() {};
		virtual ~SceneItem() {}
	};

	struct CharacterContainer;
	struct Character : SceneItem {
		CharacterContainer* attachedTo{};
		void AttachTo(CharacterContainer* tar);
		int32_t Update() override;
	};

	struct Player : Character {
	};

	struct Monster : Character {
	};

	struct CharacterContainer : SceneItem {
		xx::Listi32<xx::Weak<Character>> attachedCharacters;
		virtual void Handle(Character* c) {};
		// enter leave xxxx
	};

	struct Block : CharacterContainer {
		void Handle(Character* c) override;
	};

	struct Pole : CharacterContainer {
		void Handle(Character* c) override;
	};

	struct Scene : CharacterContainer {
		xx::Listi32<xx::Shared<SceneItem>> buildings;
		xx::Listi32<xx::Shared<Monster>> monsters;
		xx::Listi32<xx::Shared<Player>> players;
		int32_t Update() override;
		void Handle(Character* c) override;
		void Draw() override;
	};

	/************************************************************/

	inline int32_t Scene::Update() {
		for (auto i = buildings.len - 1; i >= 0; --i) {
			if (buildings[i]->Update()) {
				buildings.SwapRemoveAt(i);
			}
		}
		for (auto i = monsters.len - 1; i >= 0; --i) {
			if (monsters[i]->Update()) {
				monsters.SwapRemoveAt(i);
			}
		}
		for (auto i = players.len - 1; i >= 0; --i) {
			if (players[i]->Update()) {
				players.SwapRemoveAt(i);
			}
		}
		return 0;
	}

	inline void Scene::Draw() {
	}

	inline void Character::AttachTo(CharacterContainer* tar) {
		// assert(!tar->Contains(this));
		if (attachedTo) {
			// todo: remove from attachedTo->attachedCharacters
		}
		tar->attachedCharacters.Emplace(xx::WeakFromThis(this));
	}


	inline int32_t Character::Update() {
		attachedTo->Handle(this);
		return 0;
	}

	inline void Scene::Handle(Character* c) {
		// fly left or right
		//if (input.pressed("A or D")) c.pos.x += -= speed
	}

	inline void Block::Handle(Character* c) {
		// walk, jump, crouch ...
		//
	}

	inline void Pole::Handle(Character* c) {
		// grab? jump? walk?
		//
	}

}


namespace ActionCodes {

	/*********************************************************************************************/
	// action list enum

	enum class ActionTypes : int32_t {
		Walk,
		Atk1,
		Atk2,
		// ...
		__MaxValue__
	};
	static_assert((int32_t)ActionTypes::__MaxValue__ <= 64);

	/*********************************************************************************************/
	// base data struct

	struct alignas(8) Action {
		union {
			std::array<uint64_t, 1> _;	// need more size? increase it
			struct {
				ActionTypes type;
				int32_t __;
			};
		};
	};

	template<typename A>
	constexpr bool ActionStructCheck = alignof(Action) == alignof(A) && sizeof(A) <= sizeof(Action);

	/*********************************************************************************************/
	// action structs

	struct alignas(8) Action_Walk {
		static constexpr ActionTypes cType{ ActionTypes::Walk };
		ActionTypes type;
		int32_t n;
	};
	static_assert(ActionStructCheck<Action_Walk>);

	struct alignas(8) Action_Atk1 {
		static constexpr ActionTypes cType{ ActionTypes::Atk1 };
		ActionTypes type;
		int32_t n;
	};
	static_assert(ActionStructCheck<Action_Atk1>);

	struct alignas(8) Action_Atk2 {
		static constexpr ActionTypes cType{ ActionTypes::Atk2 };
		ActionTypes type;
		int32_t n;
	};
	static_assert(ActionStructCheck<Action_Atk2>);

	/*********************************************************************************************/
	// action user

	struct PlayerWithAction {
		int32_t actionsLen{};
		uint64_t actionFlags{};
		Action actions[2];											// todo: set more cap

		bool ActionExists(ActionTypes bt);							// return true: exists
		void ActionSetFlag(ActionTypes bt);
		void ActionClearFlag(ActionTypes bt);
		int32_t ActionFind(ActionTypes bt);							// return -1: not found
		void ActionRemove(ActionTypes bt, int32_t index);
		bool ActionTryRemove(ActionTypes bt);						// return -1: not found

		template<typename T> bool ActionExists();
		template<typename T> int32_t ActionFind();
		template<typename T> void ActionRemove(int32_t index);
		template<typename T> void ActionRemove(T& o);
		template<typename T> void ActionRemove(T&, int32_t index);	// maybe faster
		template<typename T> bool ActionTryRemove();
		template<typename...AS> void ActionTryRemoves();
		template<typename T> T& ActionAdd();
		template<typename T> T& ActionAddOrRef();

		float x{};						// player's position
		float spriteIndex{};			// 0 ~ 9: walk  10 ~ 14: atk1    15 ~ 19: atk2
		float spriteIndexBak{};

		void StepSpriteIndex(int32_t from, int32_t to, float step);

		int32_t Add_Action_Walk();
		int32_t Add_Action_Atk1();
		int32_t Add_Action_Atk2();
		// ...

		void Case_(Action_Walk& o);
		void Case_(Action_Atk1& o);
		void Case_(Action_Atk2& o);
		// ...

		void Update();
	};




	XX_INLINE bool PlayerWithAction::ActionExists(ActionTypes bt) {
		return (actionFlags & (1llu << (int32_t)bt)) > 0;
	}

	XX_INLINE void PlayerWithAction::ActionSetFlag(ActionTypes bt) {
		assert(!ActionExists(bt));
		actionFlags |= (1llu << (int32_t)bt);
	}

	XX_INLINE void PlayerWithAction::ActionClearFlag(ActionTypes bt) {
		assert(ActionExists(bt));
		actionFlags &= ~(1llu << (int32_t)bt);
	}

	// return -1 mean not found
	XX_INLINE int32_t PlayerWithAction::ActionFind(ActionTypes bt) {
		if (!ActionExists(bt)) return -1;
		for (int32_t i = actionsLen - 1; i >= 0; --i) {
			if (actions[i].type == bt) return i;
		}
		assert(false);
		return -1;
	}

	// call after ActionFind
	XX_INLINE void PlayerWithAction::ActionRemove(ActionTypes bt, int32_t index) {
		assert(ActionExists(bt));
		assert(index != -1);
		assert(index < _countof(actions));
		assert(actions[index].type == bt);
		actionFlags &= ~(1llu << (int32_t)bt);
		actions[index] = actions[--actionsLen];
	}

	// return false mean not found
	XX_INLINE bool PlayerWithAction::ActionTryRemove(ActionTypes bt) {
		if (!ActionExists(bt)) return false;
		for (int32_t index = actionsLen - 1; index >= 0; --index) {
			if (actions[index].type == bt) {
				ActionRemove(bt, index);
				return true;
			}
		}
		assert(false);
		return false;
	}

	template<typename T>
	XX_INLINE bool PlayerWithAction::ActionExists() {
		return ActionExists(T::cType);
	}

	template<typename T>
	XX_INLINE int32_t PlayerWithAction::ActionFind() {
		return ActionFind(T::cType);
	}

	template<typename T>
	XX_INLINE void PlayerWithAction::ActionRemove(int32_t index) {
		ActionRemove(T::cType, index);
	}

	template<typename T>
	XX_INLINE void PlayerWithAction::ActionRemove(T& o) {
		ActionRemove(T::cType, int32_t(((char*)&o - (char*)actions) / sizeof(Action)));
	}

	template<typename T>
	XX_INLINE void PlayerWithAction::ActionRemove(T&, int32_t index) {
		ActionRemove(T::cType, index);
	}

	template<typename T>
	XX_INLINE bool PlayerWithAction::ActionTryRemove() {
		return ActionTryRemove(T::cType);
	}

	template<typename...AS>
	XX_INLINE void PlayerWithAction::ActionTryRemoves() {
		xx::ForEachType<std::tuple<AS...>>([&]<typename T>() {
			ActionTryRemove(T::cType);
		});
	}

	template<typename T>
	XX_INLINE T& PlayerWithAction::ActionAdd() {
		assert(actionsLen < _countof(actions));
		assert(!ActionExists(T::cType));
		ActionSetFlag(T::cType);
		auto& o = (T&)actions[actionsLen++];
		o.type = T::cType;
		return o;
	}

	template<typename T>
	XX_INLINE T& PlayerWithAction::ActionAddOrRef() {
		if (!ActionExists(T::cType)) return ActionAdd<T>();
		auto index = ActionFind(T::cType);
		return (T&)actions[index];
	}

	XX_INLINE void PlayerWithAction::StepSpriteIndex(int32_t from, int32_t to, float step) {
		spriteIndex += step;
		while ((int32_t)spriteIndex > to) {
			spriteIndex = from + (spriteIndex - to);
		}
	}

	XX_INLINE int32_t PlayerWithAction::Add_Action_Walk() {
		if (ActionExists<Action_Walk>()) return 0;
		auto& o = ActionAdd<Action_Walk>();
		o.n = 0;
		return 1;
	}

	XX_INLINE int32_t PlayerWithAction::Add_Action_Atk1() {
		if (ActionExists<Action_Atk1>()) return 0;
		if (ActionExists<Action_Atk2>()) return -1;
		auto& o = ActionAdd<Action_Atk1>();
		o.n = 0;
		return 1;
	}

	XX_INLINE int32_t PlayerWithAction::Add_Action_Atk2() {
		if (ActionExists<Action_Atk2>()) return 0;
		if (ActionExists<Action_Atk1>()) return -1;
		auto& o = ActionAdd<Action_Atk2>();
		o.n = 0;
		return 1;
	}

	XX_INLINE void PlayerWithAction::Case_(Action_Walk& o) {
		if (ActionExists<Action_Atk1>() || ActionExists<Action_Atk2>()) return;
		XX_BEGIN(o.n);
		for (x = 0.f; x < 100.f; x += 100.f / 10.f / 60.f) {		// player use 10 seconds move
			StepSpriteIndex(0, 9, 10.f / 2.f / 60.f);				// 10 frame use 2 seconds play
			XX_YIELD(o.n);
		};
		for (x = 100.f; x > 0.f; x -= 100.f / 5.f / 60.f) {			// player use 5 seconds move back
			StepSpriteIndex(0, 9, 10.f / 2.f / 60.f);
			XX_YIELD(o.n);
		};
		XX_YIELD_TO_BEGIN(o.n);
		XX_END(o.n);
	}

	XX_INLINE void PlayerWithAction::Case_(Action_Atk1& o) {
		XX_BEGIN(o.n);
		spriteIndexBak = spriteIndex;
		spriteIndex = 10.f;
		do {
			XX_YIELD(o.n);
			spriteIndex += 5.f / 0.5f / 60.f;		// 5 frame play 0.5 seconds
		} while (spriteIndex < 15.f);
		spriteIndex = spriteIndexBak;
		ActionRemove(o);							// dispose self ( swap remove from actions )
		XX_END(o.n);
	}

	XX_INLINE void PlayerWithAction::Case_(Action_Atk2& o) {
		XX_BEGIN(o.n);
		spriteIndexBak = spriteIndex;
		spriteIndex = 15.f;
		do {
			XX_YIELD(o.n);
			spriteIndex += 5.f / 0.5f / 60.f;
		} while (spriteIndex < 20.f);
		spriteIndex = spriteIndexBak;
		ActionRemove(o);
		XX_END(o.n);
	}

	void PlayerWithAction::Update() {
		// execute all actions
		for (int32_t i = actionsLen - 1; i >= 0; --i) {
			auto& b = actions[i];
			switch (b.type) {
			case Action_Walk::cType: Case_((Action_Walk&)b); break;
			case Action_Atk1::cType: Case_((Action_Atk1&)b); break;
			case Action_Atk2::cType: Case_((Action_Atk2&)b); break;
				// ...
			}
		}
	}




}





namespace Stat_FullVersion {

	enum class StatTypes : uint32_t {
		// base points
		health,
		vitality,
		strength,
		dodge,
		agile,
		dexterity,
		defense,
		wisdom,
		lucky, __POINTS_END__ = lucky,

		// results
		life,					// nickname: health point / HP
		lifeRegeneration,
		energy,
		energyRegeneration,
		damageScale,
		defenseScale,
		evasion,
		movementSpeed,
		experienceScale,
		criticalChance,
		criticalBonus, __RESULTS_END__ = criticalBonus,

		// exts
		//exts1,
		//exts2,
		//exts3,
		// ...

		__MAX_VALUE__
	};

	static constexpr std::array<const char*, (uint32_t)StatTypes::__MAX_VALUE__> strings_StatTypes{
		"health",
		"vitality",
		"strength",
		"dodge",
		"agile",
		"dexterity",
		"defense",
		"wisdom",
		"lucky",

		"life",
		"lifeRegeneration",
		"energy",
		"energyRegeneration",
		"damageScale",
		"defenseScale",
		"evasion",
		"movementSpeed",
		"experienceScale",
		"criticalChance",
		"criticalBonus",

		//"exts1",
		//"exts2",
		//"exts3",
		// ...
	};

	using Stat_t = float;
	//using Stat_t = double;

	struct alignas(8) StatItem {
		StatTypes type{};
		Stat_t value{};
	};

	struct alignas(8) StatPanelBase {
		static constexpr int32_t numFields{ (int32_t)StatTypes::__POINTS_END__ + 1 };
		Stat_t health{};						// + life max, energy max
		Stat_t vitality{};						// + life regeneration, energy regeneration
		Stat_t strength{};						// + damage scale
		Stat_t dodge{};							// + evasion
		Stat_t agile{};							// + movement speed
		Stat_t dexterity{};						// + attack speed, critical bonus
		Stat_t defense{};						// + damage reduce scale
		Stat_t wisdom{};						// + experience scale
		Stat_t lucky{};							// + critical chance, improve drop rate
		constexpr Stat_t& operator[](int32_t idx) const {
			assert(idx <= SP::numFields);
			return ((Stat_t*)this)[idx];
		}
	};

	struct alignas(8) StatPanelResult {
		static constexpr int32_t numFields{ (int32_t)StatTypes::__RESULTS_END__ - (int32_t)StatTypes::__POINTS_END__ };
		Stat_t life{};							// from health			// max val
		Stat_t lifeRegeneration{};				// from vitality		// restore runtime life per seconds
		Stat_t energy{};						// from health			// max val
		Stat_t energyRegeneration{};			// from vitality		// restore runtime energy per seconds
		Stat_t damageScale{};					// from strength		// final damage = weapon damage * damageScale
		Stat_t evasion{};						// from dodge			// chance 0 ~ 1
		Stat_t movementSpeed{};					// from agile			// position change max pixel per seconds
		Stat_t attackSpeed{};					// from dexterity		// attack times per seconds
		Stat_t defenseScale{};					// from defense			// final damage = damage * defenseScale
		Stat_t experienceScale{};				// from wisdom			// final exp = exp * experienceScale
		Stat_t criticalChance{};				// from lucky			// if rand(0, 1) < criticalChance then final damage = damage * (1 + criticalBonus)
		Stat_t criticalBonus{};					// from dexterity		// for crtical hit damage calculate ( scale )
		void ZeroResult() {
			memset(this, 0, sizeof(StatPanelResult));
		}
		constexpr Stat_t& operator[](int32_t idx) const {
			assert(idx <= SP::numFields);
			return ((Stat_t*)this)[idx];
		}
	};

	struct alignas(8) StatPanel : StatPanelBase, StatPanelResult {
		static constexpr int32_t numFields{ (int32_t)StatTypes::__RESULTS_END__ + 1 };
		constexpr Stat_t& operator[](int32_t idx) const {
			assert(idx <= SP::numFields);
			return ((Stat_t*)this)[idx];
		}
		void Dump() {
			for (int32_t i = 0; i < numFields; ++i) {
				xx::CoutN(strings_StatTypes[i], " = ", operator[](i));
			}
		}
	};

	struct StatCfg {
		// rules
		static constexpr bool enableCritChanceToBonus{ false };
		static constexpr Stat_t evasionFactor{ 16 };
		static constexpr Stat_t defenseFactor{ 66 };
		static constexpr StatPanelResult rangeFrom{};	// all zero
		static constexpr StatPanelResult rangeTo{
			.life = 1000000,
			.lifeRegeneration = 1000000,
			.energy = 1000000,
			.energyRegeneration = 1000000,
			.damageScale = 1000,
			.evasion = 0.8,
			.movementSpeed = 2000,
			.attackSpeed = 100,
			.defenseScale = 0.99999,
			.experienceScale = 1000,
			.criticalChance = 1000,
			.criticalBonus = 1000,
		};
		StatPanelBase init;						// first time fill to curr value
		StatPanelBase levelRatio;				// every time when level up: curr value *= ratio
		StatPanelResult resultRatio;			// result = curr value * ratio

		template<bool fillPoint = true, bool fillLevelRatio = true, bool fillResultRatio = true>
		void Init(Stat_t defaultPoint = 0, Stat_t defaultLevelRatio = 1.1, Stat_t defaultResultRatio = 1) {
			if constexpr (fillPoint) {
				for (int32_t i = 0; i < init.numFields; ++i) {
					init[i] = defaultPoint;
				}
			}
			if constexpr (fillLevelRatio) {
				for (int32_t i = 0; i < levelRatio.numFields; ++i) {
					levelRatio[i] = defaultLevelRatio;
				}
			}
			if constexpr (fillResultRatio) {
				for (int32_t i = 0; i < resultRatio.numFields; ++i) {
					resultRatio[i] = defaultResultRatio;
				}
			}
		}

		template<typename C>
		void Calc(StatPanelBase& spb, StatPanel& sp, int32_t& currLevel, int32_t tarLevel, C const& equipments) const {
			// upgrade base points by target level
			if (currLevel == 0) {
				memcpy(&spb, &init, sizeof(spb));
				currLevel = 1;
			}
			while (currLevel < tarLevel) {
				for (int32_t i = 0; i < spb.numFields; ++i) {
					spb[i] *= levelRatio[i];
					sp[i] = spb[i];
				}
				++currLevel;
			}
			// gather points & results from items
			sp.ZeroResult();
			if (auto equipmentsCount = equipments.Count(); equipmentsCount > 0) {
				for (int32_t ei = 0; ei < equipmentsCount; ++ei) {
					auto& e = equipments[ei];
					if (auto statsCount = e->stats.Count(); statsCount > 0) {
						for (int32_t si = 0; si < statsCount; ++si) {
							auto& s = e->stats[si];
							sp[(uint32_t)s.type] += s.value;
						}
					}
				}
			}
			// calculate final results
			sp.life += sp.health * resultRatio.life;
			sp.energy += sp.health * resultRatio.energy;
			sp.lifeRegeneration += sp.vitality * resultRatio.lifeRegeneration;
			sp.energyRegeneration += sp.vitality * resultRatio.energyRegeneration;
			sp.damageScale += sp.strength * resultRatio.damageScale;
			sp.evasion += sp.dodge / (sp.dodge + this->evasionFactor);
			sp.movementSpeed += sp.agile * resultRatio.movementSpeed;
			sp.attackSpeed += sp.dexterity * resultRatio.attackSpeed;
			sp.defenseScale += sp.defense / (sp.defense + this->defenseFactor);
			sp.experienceScale += sp.wisdom * resultRatio.experienceScale;
			sp.criticalChance += sp.lucky * resultRatio.criticalChance;
			sp.criticalBonus += sp.dexterity * resultRatio.criticalBonus;
			// apply rules
			auto& spr = *(StatPanelResult*)&sp;
			for (int32_t i = 0; i < StatPanelResult::numFields; ++i) {
				auto& v = spr[i];
				if (v < rangeFrom[i]) v = rangeFrom[i];
				else if (v > rangeTo[i]) v = rangeTo[i];
			}
			if (sp.criticalChance > 1 && enableCritChanceToBonus) sp.criticalBonus += sp.criticalChance - 1;
		}
	};

	template<typename E>
	struct StatExt {
		xx::TinyList<xx::Shared<E>> equipments;
		int32_t level{}, experience{};
		Stat_t life{}, energy{};
		bool spDirty{ true };
		StatPanelBase spb;		// backup base point for level up
		StatPanel sp;
		StatCfg statCfg;

		void InitStat(int32_t tarLevel) {
			assert(level == 0);
			assert(experience == 0);
			statCfg.Calc(spb, sp, level, tarLevel, equipments);
			life = sp.life;
			energy = sp.energy;
		}
	};

};




namespace Stat_SimpleVersion {

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

	// need backup to bullet when skill create for calculate
	struct StatBase {
		StatPanel sp;
		Stat_t healthPoint{};			// left / current
		Stat_t defenseRatio{};
		Stat_t dodgeChance{};
		Stat_t movementSpeed{};
		Stat_t movementSpeedPerFrame{};
	};

	// Creature inherit this
	template<typename E>
	struct StatExt : StatBase {
		xx::TinyList<xx::Shared<E>> equipments;
		StatCfg statCfg;

		// first, need assign statCfg.init.xxxxxxxxx = xxxxxxxxxxx
		void StatCalc() {
			// gather points & results from items
			sp = statCfg.init;
			if (auto equipmentsCount = equipments.Count(); equipmentsCount > 0) {
				for (int32_t ei = 0; ei < equipmentsCount; ++ei) {
					auto& e = equipments[ei];
					if (auto statsCount = e->stats.Count(); statsCount > 0) {
						for (int32_t si = 0; si < statsCount; ++si) {
							auto& s = e->stats[si];
							sp[(uint32_t)s.type] += s.value;
						}
					}
				}
			}
			// calculate
			sp.healthRegeneration += sp.luckyPoint * statCfg.luckyToHealthRegenerationRatio;
			sp.damageRatio += sp.luckyPoint * statCfg.luckyToDamageRatio;
			sp.criticalChance += sp.luckyPoint * statCfg.luckyTocriticalChanceRatio;
			// apply rules
			for (int32_t i = 0; i < StatPanel::numFields; ++i) {
				auto& v = sp[i];
				if (v < statCfg.rangeFrom[i]) v = statCfg.rangeFrom[i];
				else if (v > statCfg.rangeTo[i]) v = statCfg.rangeTo[i];
			}
			// fill some final result
			defenseRatio = statCfg.defenseFactor / (statCfg.defenseFactor + sp.defensePoint);
			dodgeChance = sp.dodgePoint / (sp.dodgePoint + statCfg.dodgeFactor);
			movementSpeed = statCfg.baseMovementSpeed * sp.movementSpeedPoint;
			movementSpeedPerFrame = movementSpeed * Cfg::frameDelay;
		}
	};
};












/*********************************************************************************************/
/*********************************************************************************************/
// ref codes


//struct Creature;
//struct Equipment {
//	virtual ~Equipment() {}
//	xx::Weak<Creature> owner;
//	xx::TinyList<StatItem> stats;		// maybe use fixed length array instead of list
//	// ...
//};



//struct alignas(8) StatPanelExt : StatPanel {
//	Stat_t exts1;
//	Stat_t exts2;
//	Stat_t exts3;
//	// ...
//	//Stat_t dummy[1000];

//	void Clear() {
//		memset(this, 0, sizeof(StatPanelExt));
//	}

//	XX_INLINE Stat_t& At(StatTypes t) const {
//		assert((uint32_t)t <= (uint32_t)StatTypes::__MAX_VALUE__);
//		return ((Stat_t*)this)[(uint32_t)t];
//	}

//	void Dump() {
//		for (uint32_t i = 0; i <= (uint32_t)StatTypes::__MAX_VALUE__; ++i) {
//			xx::CoutN(strings_StatTypes[i], " = ", At((StatTypes)i));
//		}
//	}
//};



//struct Creature {
//	// ...

//	xx::Ref<StatCfg> statCfg;
//	xx::TinyList<xx::Shared<Equipment>> equipments;
//	Stat_t level{}, experience{};
//	Stat_t life{}, energy{};

//	bool spDirty{ true };
//	StatPanel sp;

//	void Init() {
//		level = 1;
//		experience = 0;
//		statCfg->Calc(sp, level, equipments);
//		life = sp.life;
//		energy = sp.energy;
//	}

//	void UpdateSP() {
//		if (spDirty) {
//			spDirty = false;
//			statCfg->Calc(sp, level, equipments);
//		}
//		// handle regenerations
//		if (life < sp.life) {
//			life += sp.lifeRegeneration;
//			if (life > sp.life) life = sp.life;
//		}
//		if (energy < sp.energy) {
//			energy += sp.energyRegeneration;
//			if (energy > sp.energy) energy = sp.energy;
//		}
//	}

//	void Update() {
//		UpdateSP();	// need update all monster sp first
//		// ...
//	}
//};




//struct Equipment1 : Equipment {
//	Stat_t damage{};
//	Stat_t attackSpeed{};
//	Equipment1(xx::Shared<Creature> const& owner_) {
//		owner = owner_;
//		damage = 1;
//		attackSpeed = 2;
//		stats.Emplace(StatTypes::health, 5);
//		stats.Emplace(StatTypes::vitality, 6);
//	}
//};



//struct Equipment2 : Equipment {
//	Equipment2(xx::Shared<Creature> const& owner_) {
//		owner = owner_;
//		stats.Emplace(StatTypes::movementSpeed, 100);
//	}
//};



//struct Projectile {
//	xx::Weak<Creature> owner;
//	xx::Weak<Equipment> emitter;
//	virtual ~Projectile() {};
//	virtual bool Hurt(Creature* tar) { return false; }
//};



//struct Bullet1 : Projectile {
//	Stat_t damage{};											// emitter's props copy here
//	Stat_t damageScale{}, criticalChance{}, criticalBonus{};	// owner's props copy here

//	void Init(Equipment1* emitter_) {
//		assert(emitter_);
//		owner = emitter_->owner;
//		emitter = xx::WeakFromThis(emitter_);
//		auto& sp = emitter_->owner->sp;
//		// copy props
//		damage = emitter_->damage;
//		damageScale = sp.damageScale;
//		criticalChance = sp.criticalChance;
//		criticalBonus = sp.criticalBonus;
//	}

//	// tar->life -= d ( tar->life == 0  mean  is dead )
//	bool Hurt(Creature* tar) override {
//		auto d = damage * damageScale;
//		auto crit = gLooper.rnd.Next<float>() < criticalChance;
//		if (crit) {
//			d += d * criticalBonus;
//		}
//		tar->life -= d;
//		if (tar->life <= 0) {
//			tar->life = 0;
//			// todo: emitter + exp ? update: check exp & level up ?
//		}
//		return crit;
//	}
//};



//inline void Test() {

//	xx::Listi32<xx::Shared<Creature>> chars;
//	for (int j = 0; j < 100000; j++) {
//		auto c = chars.Emplace().Emplace();
//		c->cfg.Emplace();
//		c->equipments.Emplace( xx::MakeShared<Equipment1>(c) );
//		c->equipments.Emplace( xx::MakeShared<Equipment1>(c) );
//		c->equipments.Emplace( xx::MakeShared<Equipment1>(c) );
//		c->equipments.Emplace( xx::MakeShared<Equipment2>(c) );
//		c->equipments.Emplace( xx::MakeShared<Equipment2>(c) );
//		c->equipments.Emplace( xx::MakeShared<Equipment2>(c) );
//		c->Init();
//	}
//	auto secs = xx::NowEpochSeconds();
//	for (int i = 0; i < 1000; ++i) {
//		for (int j = 0; j < 100000; j++) {
//			chars[j]->Update();
//		}
//	}
//	xx::CoutN("secs = ", xx::NowEpochSeconds(secs));

//	{
//		auto c = chars[0];
//		xx::CoutN("after Update()");
//		c->sp.Dump();
//		xx::CoutN("simulate hurt");
//		c->life -= 10;
//		xx::CoutN("life = ", c->life);
//		c->Update();
//		xx::CoutN("life = ", c->life);
//		c->Update();
//		xx::CoutN("life = ", c->life);
//		c->level++;
//		c->Update();
//		xx::CoutN("level = ", c->level);
//		xx::CoutN("life = ", c->life);
//		c->sp.Dump();
//		xx::CoutN("bullet hurt test");
//		auto b = xx::MakeShared<Bullet1>();
//		b->Init(dynamic_cast<Equipment1*>(c->equipments[0].pointer));
//		auto r = b->Hurt(c);
//		xx::CoutN("r = ", r, " life = ", c->life);
//		r = b->Hurt(c);
//		xx::CoutN("r = ", r, " life = ", c->life);
//		r = b->Hurt(c);
//		xx::CoutN("r = ", r, " life = ", c->life);
//		r = b->Hurt(c);
//		xx::CoutN("r = ", r, " life = ", c->life);
//		r = b->Hurt(c);
//		xx::CoutN("r = ", r, " life = ", c->life);
//		r = b->Hurt(c);
//		xx::CoutN("r = ", r, " life = ", c->life);
//	}
//}
