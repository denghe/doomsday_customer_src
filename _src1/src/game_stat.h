#pragma once
#include <looper.h>

namespace Game {

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
