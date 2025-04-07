#pragma once
#include <looper.h>

namespace Game {

	enum class StatTypes : uint32_t {
		// base points
		health,
		vitality,
		strength,
		agile,
		dexterity,
		defense,
		wisdom,
		lucky, __POINTS_END__ = lucky,

		// results
		life,
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
		Stat_t health{};						// + life max, energy max
		Stat_t vitality{};						// + life regeneration, energy regeneration
		Stat_t strength{};						// + damage scale
		Stat_t agile{};							// + evasion, movement speed
		Stat_t dexterity{};						// + attack speed, critical bonus
		Stat_t defense{};						// + damage reduce scale
		Stat_t wisdom{};						// + experience scale
		Stat_t lucky{};							// + critical chance, improve drop rate

		void Clear() {
			memset(this, 0, sizeof(StatPanelBase));
		}

		XX_INLINE Stat_t& At(StatTypes t) const {
			assert((uint32_t)t <= (uint32_t)StatTypes::__POINTS_END__);
			return ((Stat_t*)this)[(uint32_t)t];
		}

		void Dump() {
			for (uint32_t i = 0; i <= (uint32_t)StatTypes::__POINTS_END__; ++i) {
				xx::CoutN(strings_StatTypes[i], " = ", At((StatTypes)i));
			}
		}
	};

	struct alignas(8) StatPanel : StatPanelBase {
		Stat_t life{};							// from health			// max val
		Stat_t lifeRegeneration{};				// from vitality		// restore runtime life per seconds
		Stat_t energy{};						// from health			// max val
		Stat_t energyRegeneration{};			// from vitality		// restore runtime energy per seconds
		Stat_t damageScale{};					// from strength		// final damage = weapon damage * damageScale
		Stat_t evasion{};						// from agile			// dodge chance
		Stat_t movementSpeed{};					// from agile			// position change max pixel per seconds
		Stat_t attackSpeed{};					// from dexterity		// attack times per seconds
		Stat_t defenseScale{};					// from defense			// final damage = damage * defenseScale
		Stat_t experienceScale{};				// from wisdom			// final exp = exp * experienceScale
		Stat_t criticalChance{};				// from lucky			// if rand(0, 1) < criticalChance then final damage = damage * (1 + criticalBonus)
		Stat_t criticalBonus{};					// from dexterity		// for crtical hit damage calculate ( scale )

		void Clear() {
			memset(this, 0, sizeof(StatPanel));
		}

		XX_INLINE Stat_t& At(StatTypes t) const {
			assert((uint32_t)t <= (uint32_t)StatTypes::__RESULTS_END__);
			return ((Stat_t*)this)[(uint32_t)t];
		}

		void Dump() {
			for (uint32_t i = 0; i <= (uint32_t)StatTypes::__RESULTS_END__; ++i) {
				xx::CoutN(strings_StatTypes[i], " = ", At((StatTypes)i));
			}
		}
	};



	struct Creature;
	struct Equipment {
		virtual ~Equipment() {}
		xx::Weak<Creature> owner;
		xx::TinyList<StatItem> stats;		// maybe use fixed length array instead of list
		// ...
	};

	struct StatCfg {
		// initial values
		Stat_t initHealth{10};
		Stat_t initVitality{10};
		Stat_t initStrength{10};
		Stat_t initAgile{10};
		Stat_t initDexterity{10};
		Stat_t initDefense{10};
		Stat_t initWisdom{10};
		Stat_t initLucky{10};

		// level up ratios
		Stat_t levelToHealthRatio{2};
		Stat_t levelToVitalityRatio{2};
		Stat_t levelToStrengthRatio{2};
		Stat_t levelToAgileRatio{2};
		Stat_t levelToDexterityRatio{2};
		Stat_t levelToDefenseRatio{2};
		Stat_t levelToWisdomRatio{2};
		Stat_t levelToLuckyRatio{2};

		// base to result ratios
		Stat_t healthToLifeRatio{2};
		Stat_t healthToEnergyRatio{5};
		Stat_t vitalityToLifeRegenerationRatio{0.2};
		Stat_t vitalityToEnergyRegenerationRatio{0.5};
		Stat_t strengthToDamageScaleRatio{0.1};
		Stat_t evasionFactor{100};
		Stat_t baseMovementSpeed{300};
		Stat_t agileToMovementSpeedRatio{0.1};
		Stat_t dexterityToAttackSpeedRatio{0.1};
		Stat_t dexterityToCritialBonusScaleRatio{0.1};
		Stat_t defenseFactor{100};
		Stat_t wisdomToExperienceScaleRatio{0.1};
		Stat_t luckyToCritialChanceScaleRatio{0.01};

		// rules
		bool enableCritChanceToBonus{ false };
		xx::FromTo<Stat_t> rangeLife{ 1, 1000000 };
		xx::FromTo<Stat_t> rangeLifeRegeneration{ 1, 1000000 };
		xx::FromTo<Stat_t> rangeEnergy{ 1, 1000000 };
		xx::FromTo<Stat_t> rangeEnergyRegeneration{ 1, 1000000 };
		xx::FromTo<Stat_t> rangeDamageScale{ 0, 1000000 };
		xx::FromTo<Stat_t> rangeEvasion{ 0, 0.99999 };
		xx::FromTo<Stat_t> rangeMovementSpeed{ 0, Cfg::unitSize * Cfg::fps * 0.5 };
		xx::FromTo<Stat_t> rangeAttackSpeed{ 0, 120 };
		xx::FromTo<Stat_t> rangeDefenseScale{ 0, 0.99999 };
		xx::FromTo<Stat_t> rangeExperienceScale{ 1, 1000000 };
		xx::FromTo<Stat_t> rangeCriticalChance{ 0, 1000000 };
		xx::FromTo<Stat_t> rangeCriticalBonus{ 0, 1000000 };

		// requires C:: Count() & operator[]
		// level: runtime int32. range: 1 ~ xxxx
		template<typename C>
		void Calc(StatPanel& sp, int32_t level, C const& equipments) const {
			sp.Clear();
			// calculate points by level
			sp.health = this->initHealth + (level - 1) * this->levelToHealthRatio;
			sp.vitality = this->initVitality + (level - 1) * this->levelToVitalityRatio;
			sp.strength = this->initStrength + (level - 1) * this->levelToStrengthRatio;
			sp.agile = this->initAgile + (level - 1) * this->levelToAgileRatio;
			sp.dexterity = this->initDexterity + (level - 1) * this->levelToDexterityRatio;
			sp.defense = this->initDefense + (level - 1) * this->levelToDefenseRatio;
			sp.wisdom = this->initWisdom + (level - 1) * this->levelToWisdomRatio;
			sp.lucky = this->initLucky + (level - 1) * this->levelToLuckyRatio;
			// gather points & results from items
			if (auto equipmentsCount = equipments.Count(); equipmentsCount > 0) {
				for (int32_t ei = 0; ei < equipmentsCount; ++ei) {
					auto& e = equipments[ei];
					if (auto statsCount = e->stats.Count(); statsCount > 0) {
						for (int32_t si = 0; si < statsCount; ++si) {
							auto& s = e->stats[si];
							sp.At(s.type) += s.value;
						}
					}
				}
			}
			// calculate final results
			sp.life += sp.health * this->healthToLifeRatio;
			sp.energy += sp.health * this->healthToEnergyRatio;
			sp.lifeRegeneration += sp.vitality * this->vitalityToLifeRegenerationRatio;
			sp.energyRegeneration += sp.vitality * this->vitalityToEnergyRegenerationRatio;
			sp.damageScale += sp.strength * this->strengthToDamageScaleRatio;
			sp.evasion += 1 - this->evasionFactor / (this->evasionFactor + sp.agile);
			sp.movementSpeed += this->baseMovementSpeed + sp.agile * this->agileToMovementSpeedRatio;
			sp.attackSpeed += sp.dexterity * this->dexterityToAttackSpeedRatio;
			sp.defenseScale += this->defenseFactor / (this->defenseFactor + sp.defense);
			sp.experienceScale += sp.wisdom * this->wisdomToExperienceScaleRatio;
			sp.criticalChance += sp.lucky * this->luckyToCritialChanceScaleRatio;
			sp.criticalBonus += sp.dexterity * this->dexterityToCritialBonusScaleRatio;
			// handle rules
			this->rangeLife.Limit(sp.life);
			this->rangeLifeRegeneration.Limit(sp.lifeRegeneration);
			this->rangeEnergy.Limit(sp.energy);
			this->rangeEnergyRegeneration.Limit(sp.energyRegeneration);
			this->rangeDamageScale.Limit(sp.damageScale);
			this->rangeEvasion.Limit(sp.evasion);
			this->rangeMovementSpeed.Limit(sp.movementSpeed);
			this->rangeAttackSpeed.Limit(sp.attackSpeed);
			this->rangeDefenseScale.Limit(sp.defenseScale);
			this->rangeExperienceScale.Limit(sp.experienceScale);
			this->rangeCriticalChance.Limit(sp.criticalChance);
			this->rangeCriticalBonus.Limit(sp.criticalBonus);
			if (sp.criticalChance > 1 && enableCritChanceToBonus) sp.criticalBonus += sp.criticalChance - 1;
		}
	};

};

/*********************************************************************************************/
/*********************************************************************************************/
// ref codes



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

//	xx::Ref<StatCfg> cfg;
//	xx::TinyList<xx::Shared<Equipment>> equipments;
//	Stat_t level{}, experience{};
//	Stat_t life{}, energy{};

//	bool spDirty{ true };
//	StatPanel sp;

//	void Init() {
//		level = 1;
//		experience = 0;
//		UpdateSP();
//		life = sp.life;
//		energy = sp.energy;
//	}

//	void UpdateSP() {
//		if (spDirty) {
//			spDirty = false;
//			cfg->Calc(sp, equipments);
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
