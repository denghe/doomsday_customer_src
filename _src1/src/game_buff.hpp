#pragma once

namespace Game {

	template<> void BuffAddTo<BuffTypes::__Default__>(BuffContainer& bc) {
		assert(false);
	}

	// 攻击+20%，攻速-5%，最大血量-5%
	template<> void BuffAddTo<BuffTypes::_996>(BuffContainer& bc) {
		bc.sp.damageRatio += 0.2f;
		bc.sp.attackSpeedRatio -= 0.05f;
		bc.sp.healthRatio -= 0.05f;
	}

	// 血量回复+10/s
	template<> void BuffAddTo<BuffTypes::中药调理>(BuffContainer& bc) {
		bc.sp.healthRegeneration += 10;
	}

	// 最大血量+15%，下一回合以30%血量起步
	template<> void BuffAddTo<BuffTypes::跑步机>(BuffContainer& bc) {
		bc.sp.healthRatio += 0.15f;
		bc._跑步机_used = false;
	}

	// 范围+10，幸运-5
	template<> void BuffAddTo<BuffTypes::眼镜>(BuffContainer& bc) {
		bc.sp.fieldOfVision += 10;
		bc.sp.luckyPoint -= 5;
	}

	// 射速+5%
	template<> void BuffAddTo<BuffTypes::肘击>(BuffContainer& bc) {
		bc.sp.attackSpeedRatio += 0.05f;
	}

	// 攻击+15%，血量回复-5/s
	template<> void BuffAddTo<BuffTypes::加班餐>(BuffContainer& bc) {
		bc.sp.damageRatio += 0.15f;
		bc.sp.healthRegeneration -= 5;
	}

	// 移速+1
	template<> void BuffAddTo<BuffTypes::冲刺鞋子>(BuffContainer& bc) {
		bc.sp.movementSpeedPoint += 1;
	}

	// 攻速+20%，移速-1
	template<> void BuffAddTo<BuffTypes::旋转办公椅>(BuffContainer& bc) {
		bc.sp.attackSpeedRatio += 0.2f;
		bc.sp.movementSpeedPoint -= 1;
	}

	// 攻速+10%，幸运-5，收获+5%
	template<> void BuffAddTo<BuffTypes::塑料友情>(BuffContainer& bc) {
		bc.sp.attackSpeedRatio += 0.1f;
		bc.sp.luckyPoint -= 5;
		bc.sp.harvestRatio += 0.05f;
	}

	// 护甲+5
	template<> void BuffAddTo<BuffTypes::钢板>(BuffContainer& bc) {
		bc.sp.defensePoint += 5;
	}

	// 最大血量+5%，血量回复+10，伤害-5%
	template<> void BuffAddTo<BuffTypes::海底捞>(BuffContainer& bc) {
		bc.sp.healthRatio += 0.05f;
		bc.sp.healthRegeneration += 10;
		bc.sp.damageRatio -= 0.05f;
	}

	// 暴击伤害+15%
	template<> void BuffAddTo<BuffTypes::义愤填膺>(BuffContainer& bc) {
		bc.sp.criticalBonusRatio += 0.15f;
	}

	// 杀死小怪时有70%概率+30HP，30%概率-30HP
	template<> void BuffAddTo<BuffTypes::国潮外卖>(BuffContainer& bc) {
	}

	// 攻击-20%，幸运+30，血量回复+15/s
	template<> void BuffAddTo<BuffTypes::躺平>(BuffContainer& bc) {
		bc.sp.damageRatio -= 0.2f;
		bc.sp.luckyPoint += 30;
		bc.sp.healthRegeneration += 15;
	}

	// 攻击+30%，攻速+10%，每5秒不受控制的固定向前方冲刺0.5秒
	template<> void BuffAddTo<BuffTypes::社交牛逼症>(BuffContainer& bc) {
		bc.sp.damageRatio += 0.3f;
		bc.sp.attackSpeedRatio += 0.1f;
		// todo: register player auto control skill?
		//bc.owner->skills.Add()
	}

	// 幸运+40，攻击-5%
	template<> void BuffAddTo<BuffTypes::风水大师>(BuffContainer& bc) {
		bc.sp.luckyPoint += 40;
		bc.sp.damageRatio -= 0.05f;
	}

	// 攻击-5%，攻速+30%
	template<> void BuffAddTo<BuffTypes::CRUD糕手>(BuffContainer& bc) {
		bc.sp.damageRatio -= 0.05f;
		bc.sp.attackSpeedRatio += 0.3f;
	}

	// 幸运+20，移速-1，血量回复+15/s
	template<> void BuffAddTo<BuffTypes::动漫达人>(BuffContainer& bc) {
		bc.sp.luckyPoint += 20;
		bc.sp.movementSpeedPoint -= 1;
		bc.sp.healthRegeneration += 15;
	}

	// 攻击+15%，攻速+30%，血量回复-20/s
	template<> void BuffAddTo<BuffTypes::屎山代码>(BuffContainer& bc) {
		bc.sp.damageRatio += 0.15f;
		bc.sp.attackSpeedRatio += 0.3f;
		bc.sp.healthRegeneration -= 20;
	}

	// 最大血量-50%，攻速+100%，攻击-20%
	template<> void BuffAddTo<BuffTypes::福报>(BuffContainer& bc) {
		bc.sp.healthRatio -= 0.5f;
		bc.sp.attackSpeedRatio += 1;
		bc.sp.damageRatio -= 0.2f;
	}

	// 移速+2，闪避+3%
	template<> void BuffAddTo<BuffTypes::归宅部>(BuffContainer& bc) {
		bc.sp.movementSpeedPoint += 2;
		bc.sp.dodgeRatio += 0.03f;
	}

	// 攻击+3%，幸运+5，血量回复+5/s，攻速+5%，最大血量+5%，暴击率+3%，爆伤+5%
	template<> void BuffAddTo<BuffTypes::为什么我不是沪爷>(BuffContainer& bc) {
		bc.sp.damageRatio += 0.03f;
		bc.sp.luckyPoint += 5;
		bc.sp.healthRegeneration += 5;
		bc.sp.attackSpeedRatio += 0.05f;
		bc.sp.healthRatio += 0.05f;
		bc.sp.criticalChance += 0.03f;
		bc.sp.criticalBonusRatio += 0.05f;
	}

	// 幸运 +60，攻击 -10%，血量回复 -5/s
	template<> void BuffAddTo<BuffTypes::彩票研究者>(BuffContainer& bc) {
		bc.sp.luckyPoint += 60;
		bc.sp.damageRatio -= 0.1f;
		bc.sp.healthRegeneration -= 5;
	}

	// 闪避+15%，攻速 -10%，每3秒强制向后方冲刺0.5秒
	template<> void BuffAddTo<BuffTypes::社恐>(BuffContainer& bc) {
		bc.sp.dodgeRatio += 0.15f;
		bc.sp.attackSpeedRatio -= 0.1f;
		// todo: register player auto control skill?
		//bc.owner->skills.Add()
	}

	// 杀死小怪时攻击+3%（一局内）
	template<> void BuffAddTo<BuffTypes::肉食主义>(BuffContainer& bc) {
	}

	// 按下特殊键F进入10秒无敌状态，但是不能行动。一局一次
	template<> void BuffAddTo<BuffTypes::请假>(BuffContainer& bc) {
		// todo: register player skill?
		//bc.owner->skills.Add()
	}

	// 攻击+40%，攻速+20%，每回合最大HP-10%，收获-10%
	template<> void BuffAddTo<BuffTypes::技术债>(BuffContainer& bc) {
		bc.sp.damageRatio += 0.4f;
		bc.sp.attackSpeedRatio += 0.2f;
	}

	// 攻击射出的子弹额外附加30%幸运值倍率的伤害
	template<> void BuffAddTo<BuffTypes::南无加特林机枪菩萨>(BuffContainer& bc) {
	}

	// 最大血量提升，提升值为300%幸运值
	template<> void BuffAddTo<BuffTypes::南无阿弥陀佛>(BuffContainer& bc) {
	}

	// 攻击-3%，幸运-5，血量回复-5/s，攻速-5%，最大血量-5%，暴击率-3%，爆伤-5%，收获+200
	template<> void BuffAddTo<BuffTypes::沪爷>(BuffContainer& bc) {
		bc.sp.damageRatio -= 0.03f;
		bc.sp.luckyPoint -= 5;
		bc.sp.healthRegeneration -= 5;
		bc.sp.attackSpeedRatio -= 0.05f;
		bc.sp.healthRatio -= 0.05f;
		bc.sp.criticalChance -= 0.03f;
		bc.sp.criticalBonusRatio -= 0.05f;
		bc.sp.harvestRatio += 2;
	}

	// 按F生成一个好兄弟，好兄弟会进入15秒无敌状态，和玩家拥有相同的攻击力，怪物会优先攻击好兄弟。15秒后结束。一局一次
	template<> void BuffAddTo<BuffTypes::坐在后面的好兄弟>(BuffContainer& bc) {
		// todo: register player skill?
		//bc.owner->skills.Add()
	}

	// 生成3个好兄弟，拥有玩家50%攻击力和100%血量。自身攻击-25%，移速-1
	template<> void BuffAddTo<BuffTypes::团队Leader>(BuffContainer& bc) {
		bc.sp.damageRatio -= 0.25f;
		bc.sp.movementSpeedPoint -= 1;
		// todo: register player skill( auto cast & only once )
		//bc.owner->skills.Add()
	}

	// 移速+5，护甲+30，攻击-100%
	template<> void BuffAddTo<BuffTypes::sudo_rm_rf>(BuffContainer& bc) {
		bc.sp.movementSpeedPoint += 5;
		bc.sp.defensePoint += 30;
		bc.sp.damageRatio -= 1;
	}

	// 死亡时回复20%HP保证不死，一局一次。触发时对全体怪物造成100%幸运值+300%倍率的伤害。
	template<> void BuffAddTo<BuffTypes::裁员大动脉>(BuffContainer& bc) {
	}

	// 最大血量+50%，伤害+50%，下一回合以1%血量起步
	template<> void BuffAddTo<BuffTypes::筋肉兄贵>(BuffContainer& bc) {
		bc.sp.healthRatio += 0.5f;
		bc.sp.damageRatio += 0.5f;
		bc._筋肉兄贵_used = false;
	}

	// 每回合结束时-50%~+100%代币
	template<> void BuffAddTo<BuffTypes::投资>(BuffContainer& bc) {
	}

	// 无作用
	template<> void BuffAddTo<BuffTypes::劳动法>(BuffContainer& bc) {}


	inline void BuffContainer::Init(Creature* owner_) {
		owner = xx::WeakFromThis(owner_);
		memset(&nums, 0, sizeof(nums));
		sp.Clear();
		FillRanks();
	}

	XX_INLINE int32_t& BuffContainer::At(BuffTypes bt) const {
		return (int32_t&)nums[(int32_t)bt];
	}

	XX_INLINE bool BuffContainer::IsLimited(BuffTypes bt) const {
		assert(At(bt) <= BuffLimits[(int32_t)bt]);
		return At(bt) == BuffLimits[(int32_t)bt];
	}

	// for shopping. return value for assert
	inline bool BuffContainer::TryAdd(BuffTypes bt) {
		auto i = (int32_t)bt;
		if (nums[i] < BuffLimits[i]) {
			++nums[i];
			BuffAddTos[i](*this);
			logs.emplace_back(bt);
			return true;
		}
		return false;
	}
	
	inline void BuffContainer::FillRanks() {
		auto v = owner->sp.luckyPoint;
		if (lastLuckyVal == v) return;
		for (int i = 1; i < numBuffers; ++i) {
			auto r = BuffRanks[i];
			if (r == 1) {
				ranks[i] = 70 - 60 * (v * 0.001f);
			}
			else if (r == 2) {
				ranks[i] = 20 + 40 * (v * 0.001f);
			}
			else {
				ranks[i] = 10 + 20 * (v * 0.001f);
			}
		}
		lastLuckyVal = v;
	}

	inline int32_t BuffContainer::GetShopBuffs(BuffTypes* buff, int32_t buffLen) {
		assert(buff && buffLen > 0);
		FillRanks();
		auto& rnd = owner->stage->rnd;
		std::array<std::pair<BuffTypes, Stat_t>, numBuffers + 1> steps;		// tmp container
		steps[0] = { BuffTypes::__Default__, (Stat_t)0 };
		auto ns = nums;								// copy for calc
		int32_t rtv{};														// filled len
		for (int j = 0; j < buffLen; j++) {
			int n{ 1 };
			for (int i = 1; i < numBuffers; ++i) {							// fill avaliable buffs into steps
				if (ns[i] < BuffLimits[i]) {
					steps[n].first = (BuffTypes)i;
					steps[n].second = steps[n - 1].second + ranks[i];
					++n;
				}
			}
			steps[n] = { steps[n - 1].first, steps[n - 1].second + 1 };		// for easy compare. +1 ensure bigger than last

			if (n > 1) {
				auto tar = rnd.Next<Stat_t>(steps[n - 1].second);
				for (int i = 1; i <= n; ++i) {
					if (steps[i].second > tar) {
						++ns[(int32_t)steps[i].first];
						buff[rtv] = steps[i].first;
						++rtv;
						break;
					}
				}
			}
			else return rtv;
		}
		return rtv;
	}

}
