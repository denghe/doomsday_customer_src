#pragma once

namespace Game {

	enum class BuffTypes : int32_t {
		__Default__ = 0,
		_996,
		中药调理,
		跑步机,
		眼镜,
		肘击,
		加班餐,
		冲刺鞋子,
		旋转办公椅,
		塑料友情,
		钢板,
		海底捞,
		义愤填膺,
		国潮外卖,
		躺平,
		社交牛逼症,
		风水大师,
		CRUD糕手,
		动漫达人,
		屎山代码,
		福报,
		归宅部,
		为什么我不是沪爷,
		彩票研究者,
		社恐,
		肉食主义,
		请假,
		技术债,
		南无加特林机枪菩萨,
		南无阿弥陀佛,
		沪爷,
		坐在后面的好兄弟,
		团队Leader,
		sudo_rm_rf,
		裁员大动脉,
		筋肉兄贵,
		投资,
		劳动法,
		__MaxValue__
	};

	static constexpr int32_t BuffLimits[(int32_t)BuffTypes::__MaxValue__] = {
		0, // __Default__ = 0,
		999, // _996,
		999, // 中药调理,
		999, // 跑步机,
		999, // 眼镜,
		999, // 肘击,
		999, // 加班餐,
		10, // 冲刺鞋子,
		5, // 旋转办公椅,
		999, // 塑料友情,
		999, // 钢板,
		999, // 海底捞,
		999, // 义愤填膺,
		1, // 国潮外卖,
		1, // 躺平,
		1, // 社交牛逼症,
		999, // 风水大师,
		999, // CRUD糕手,
		999, // 动漫达人,
		999, // 屎山代码,
		1, // 福报,
		3, // 归宅部,
		999, // 为什么我不是沪爷,
		1, // 彩票研究者,
		1, // 社恐,
		999, // 肉食主义,
		1, // 请假,
		999, // 技术债,
		1, // 南无加特林机枪菩萨,
		1, // 南无阿弥陀佛,
		999, // 沪爷,
		1, // 坐在后面的好兄弟,
		1, // 团队Leader,
		1, // sudo_rm_rf,
		1, // 裁员大动脉,
		999, // 筋肉兄贵,
		1, // 投资,
		1, // 劳动法,
	};

	static constexpr int32_t BuffRanks[(int32_t)BuffTypes::__MaxValue__] = {
		0, // __Default__ = 0,
		1, // _996,
		1, // 中药调理,
		1, // 跑步机,
		1, // 眼镜,
		1, // 肘击,
		1, // 加班餐,
		1, // 冲刺鞋子,
		1, // 旋转办公椅,
		1, // 塑料友情,
		1, // 钢板,
		1, // 海底捞,
		1, // 义愤填膺,
		1, // 国潮外卖,
		2, // 躺平,
		2, // 社交牛逼症,
		2, // 风水大师,
		2, // CRUD糕手,
		2, // 动漫达人,
		2, // 屎山代码,
		2, // 福报,
		2, // 归宅部,
		2, // 为什么我不是沪爷,
		2, // 彩票研究者,
		2, // 社恐,
		2, // 肉食主义,
		2, // 请假,
		2, // 技术债,
		3, // 南无加特林机枪菩萨,
		3, // 南无阿弥陀佛,
		3, // 沪爷,
		3, // 坐在后面的好兄弟,
		3, // 团队Leader,
		3, // sudo_rm_rf,
		3, // 裁员大动脉,
		3, // 筋肉兄贵,
		3, // 投资,
		3, // 劳动法,
	};

	template<BuffTypes bt> void BuffAddToSP(StatPanel& sp);
	template<> void BuffAddToSP<BuffTypes::__Default__>(StatPanel& sp) {}

	// 攻击+20%，攻速-5%，最大血量-5%
	template<> void BuffAddToSP<BuffTypes::_996>(StatPanel& sp) {
		sp.damageRatio += 0.2f;
		sp.attackSpeed -= 0.05f;
		sp.healthRatio -= 0.05f;
	}

	// 血量回复+10/s
	template<> void BuffAddToSP<BuffTypes::中药调理>(StatPanel& sp) {
		sp.healthRegeneration += 10;
	}

	// 最大血量+15%，下一回合以30%血量起步
	template<> void BuffAddToSP<BuffTypes::跑步机>(StatPanel& sp) {
		sp.healthRatio += 0.15f;
		// todo: register round begin event?
	}

	// 范围+10，幸运-5
	template<> void BuffAddToSP<BuffTypes::眼镜>(StatPanel& sp) {
		sp.cameraZoom += 0.1f;
		sp.luckyPoint -= 5;
	}

	// 射速+5%
	template<> void BuffAddToSP<BuffTypes::肘击>(StatPanel& sp) {
		sp.attackSpeed += 0.05f;
	}

	// 攻击+15%，血量回复-5/s
	template<> void BuffAddToSP<BuffTypes::加班餐>(StatPanel& sp) {
		sp.damageRatio += 0.15f;
		sp.healthRegeneration -= 5;
	}

	// 移速+1
	template<> void BuffAddToSP<BuffTypes::冲刺鞋子>(StatPanel& sp) {
		sp.movementSpeedPoint += 1;
	}

	// 攻速+20%，移速-1
	template<> void BuffAddToSP<BuffTypes::旋转办公椅>(StatPanel& sp) {
		sp.attackSpeed += 0.2f;
		sp.movementSpeedPoint -= 1;
	}

	// 攻速+10%，幸运-5，收获+5%
	template<> void BuffAddToSP<BuffTypes::塑料友情>(StatPanel& sp) {
		sp.attackSpeed += 0.1f;
		sp.luckyPoint -= 5;
		sp.harvestRatio += 0.05f;
	}

	// 护甲+5
	template<> void BuffAddToSP<BuffTypes::钢板>(StatPanel& sp) {
		sp.defensePoint += 5;
	}

	// 最大血量+5%，血量回复+10，伤害-5%
	template<> void BuffAddToSP<BuffTypes::海底捞>(StatPanel& sp) {
		sp.healthRatio += 0.05f;
		sp.healthRegeneration += 10;
		sp.damageRatio -= 0.05f;
	}

	// 暴击伤害+15%
	template<> void BuffAddToSP<BuffTypes::义愤填膺>(StatPanel& sp) {
		sp.criticalBonusRatio += 0.15f;
	}

	// 杀死小怪时有70%概率+30HP，30%概率-30HP
	template<> void BuffAddToSP<BuffTypes::国潮外卖>(StatPanel& sp) {
		// todo: register kill monster event?
	}

	// 攻击-20%，幸运+30，血量回复+15/s
	template<> void BuffAddToSP<BuffTypes::躺平>(StatPanel& sp) {
		sp.damageRatio -= 0.2f;
		sp.luckyPoint += 30;
		sp.healthRegeneration += 15;
	}

	// 攻击+30%，攻速+10%，每5秒不受控制的固定向前方冲刺0.5秒
	template<> void BuffAddToSP<BuffTypes::社交牛逼症>(StatPanel& sp) {
		sp.damageRatio += 0.3f;
		sp.attackSpeed += 0.1f;
		// todo: register player auto control skill?
	}

	// 幸运+40，攻击-5%
	template<> void BuffAddToSP<BuffTypes::风水大师>(StatPanel& sp) {
		sp.luckyPoint += 40;
		sp.damageRatio -= 0.05f;
	}

	// 攻击-5%，攻速+30%
	template<> void BuffAddToSP<BuffTypes::CRUD糕手>(StatPanel& sp) {
		sp.damageRatio -= 0.05f;
		sp.attackSpeed += 0.3f;
	}

	// 幸运+20，移速-1，血量回复+15/s
	template<> void BuffAddToSP<BuffTypes::动漫达人>(StatPanel& sp) {
		sp.luckyPoint += 20;
		sp.movementSpeedPoint -= 1;
		sp.healthRegeneration += 15;
	}

	// 攻击+15%，攻速+30%，血量回复-20/s
	template<> void BuffAddToSP<BuffTypes::屎山代码>(StatPanel& sp) {
		sp.damageRatio += 0.15f;
		sp.attackSpeed += 0.3f;
		sp.healthRegeneration -= 20;
	}

	// 最大血量-50%，攻速+100%，攻击-20%
	template<> void BuffAddToSP<BuffTypes::福报>(StatPanel& sp) {
		sp.healthRatio -= 0.5f;
		sp.attackSpeed += 1;
		sp.damageRatio -= 0.2f;
	}

	// 移速+2，闪避+3%
	template<> void BuffAddToSP<BuffTypes::归宅部>(StatPanel& sp) {
		sp.movementSpeedPoint += 2;
		sp.dodgeRatio += 0.03f;
	}

	// 攻击+3%，幸运+5，血量回复+5/s，攻速+5%，最大血量+5%，暴击率+3%，爆伤+5%
	template<> void BuffAddToSP<BuffTypes::为什么我不是沪爷>(StatPanel& sp) {
		sp.damageRatio += 0.03f;
		sp.luckyPoint += 5;
		sp.healthRegeneration += 5;
		sp.attackSpeed += 0.05f;
		sp.healthRatio += 0.05f;
		sp.criticalChance += 0.03f;
		sp.criticalBonusRatio += 0.05f;
	}

	// 幸运 +60，攻击 -10%，血量回复 -5/s
	template<> void BuffAddToSP<BuffTypes::彩票研究者>(StatPanel& sp) {
		sp.luckyPoint += 60;
		sp.damageRatio -= 0.1f;
		sp.healthRegeneration -= 5;
	}

	// 闪避+15%，攻速 -10%，每3秒强制向后方冲刺0.5秒
	template<> void BuffAddToSP<BuffTypes::社恐>(StatPanel& sp) {
		sp.dodgeRatio += 0.15f;
		sp.attackSpeed -= 0.1f;
		// todo: register player auto control skill?
	}

	// 杀死小怪时攻击+3%（一局内）
	template<> void BuffAddToSP<BuffTypes::肉食主义>(StatPanel& sp) {
		// todo: register monster dead event? add extra store field?
	}

	// 按下特殊键F进入10秒无敌状态，但是不能行动。一局一次
	template<> void BuffAddToSP<BuffTypes::请假>(StatPanel& sp) {
		// todo: register player skill?
	}

	// 攻击+40%，攻速+20%，每回合最大HP-10%，收获-10%
	template<> void BuffAddToSP<BuffTypes::技术债>(StatPanel& sp) {
		sp.damageRatio += 0.4f;
		sp.attackSpeed += 0.2f;
		// todo: register round begin event? add extra store field?
	}

	// 攻击射出的子弹额外附加30%幸运值倍率的伤害
	template<> void BuffAddToSP<BuffTypes::南无加特林机枪菩萨>(StatPanel& sp) {
		// todo: add extra store field?
	}

	// 最大血量提升，提升值为300%幸运值
	template<> void BuffAddToSP<BuffTypes::南无阿弥陀佛>(StatPanel& sp) {
		// todo: add extra store field?
	}

	// 攻击-3%，幸运-5，血量回复-5/s，攻速-5%，最大血量-5%，暴击率-3%，爆伤-5%，收获+200
	template<> void BuffAddToSP<BuffTypes::沪爷>(StatPanel& sp) {
		sp.damageRatio -= 0.03f;
		sp.luckyPoint -= 5;
		sp.healthRegeneration -= 5;
		sp.attackSpeed -= 0.05f;
		sp.healthRatio -= 0.05f;
		sp.criticalChance -= 0.03f;
		sp.criticalBonusRatio -= 0.05f;
		sp.harvestRatio += 2;
	}

	// 按F生成一个好兄弟，好兄弟会进入15秒无敌状态，和玩家拥有相同的攻击力，怪物会优先攻击好兄弟。15秒后结束。一局一次
	template<> void BuffAddToSP<BuffTypes::坐在后面的好兄弟>(StatPanel& sp) {
		// todo: register player skill?
	}

	// 生成3个好兄弟，拥有玩家50%攻击力和100%血量。自身攻击-25%，移速-1
	template<> void BuffAddToSP<BuffTypes::团队Leader>(StatPanel& sp) {
		// todo: register round begin event?
		sp.damageRatio -= 0.25f;
		sp.movementSpeedPoint -= 1;
	}

	// 移速+5，护甲+30，攻击-100%
	template<> void BuffAddToSP<BuffTypes::sudo_rm_rf>(StatPanel& sp) {
		sp.movementSpeedPoint += 5;
		sp.defensePoint += 30;
		sp.damageRatio -= 1;
	}

	// 死亡时回复20%HP保证不死，一局一次。触发时对全体怪物造成100%幸运值+300%倍率的伤害。
	template<> void BuffAddToSP<BuffTypes::裁员大动脉>(StatPanel& sp) {
		// todo: register player before dead event?
	}

	// 最大血量+50%，伤害+50%，下一回合以1%血量起步
	template<> void BuffAddToSP<BuffTypes::筋肉兄贵>(StatPanel& sp) {
		sp.healthRatio += 0.5f;
		sp.damageRatio += 0.5f;
		// todo: register round begin event?
	}

	// 每回合结束时-50%~+100%代币
	template<> void BuffAddToSP<BuffTypes::投资>(StatPanel& sp) {
		// todo: register round end ( before buy buff ) event?
	}

	// 无作用
	template<> void BuffAddToSP<BuffTypes::劳动法>(StatPanel& sp) {}

	typedef void (*Func_BuffAddToSP)(StatPanel&);
	static constexpr Func_BuffAddToSP BuffAddToSPs[(int32_t)BuffTypes::__MaxValue__] = {
		BuffAddToSP<BuffTypes::__Default__>,
		BuffAddToSP<BuffTypes::_996>,
		BuffAddToSP<BuffTypes::中药调理>,
		BuffAddToSP<BuffTypes::跑步机>,
		BuffAddToSP<BuffTypes::眼镜>,
		BuffAddToSP<BuffTypes::肘击>,
		BuffAddToSP<BuffTypes::加班餐>,
		BuffAddToSP<BuffTypes::冲刺鞋子>,
		BuffAddToSP<BuffTypes::旋转办公椅>,
		BuffAddToSP<BuffTypes::塑料友情>,
		BuffAddToSP<BuffTypes::钢板>,
		BuffAddToSP<BuffTypes::海底捞>,
		BuffAddToSP<BuffTypes::义愤填膺>,
		BuffAddToSP<BuffTypes::国潮外卖>,
		BuffAddToSP<BuffTypes::躺平>,
		BuffAddToSP<BuffTypes::社交牛逼症>,
		BuffAddToSP<BuffTypes::风水大师>,
		BuffAddToSP<BuffTypes::CRUD糕手>,
		BuffAddToSP<BuffTypes::动漫达人>,
		BuffAddToSP<BuffTypes::屎山代码>,
		BuffAddToSP<BuffTypes::福报>,
		BuffAddToSP<BuffTypes::归宅部>,
		BuffAddToSP<BuffTypes::为什么我不是沪爷>,
		BuffAddToSP<BuffTypes::彩票研究者>,
		BuffAddToSP<BuffTypes::社恐>,
		BuffAddToSP<BuffTypes::肉食主义>,
		BuffAddToSP<BuffTypes::请假>,
		BuffAddToSP<BuffTypes::技术债>,
		BuffAddToSP<BuffTypes::南无加特林机枪菩萨>,
		BuffAddToSP<BuffTypes::南无阿弥陀佛>,
		BuffAddToSP<BuffTypes::沪爷>,
		BuffAddToSP<BuffTypes::坐在后面的好兄弟>,
		BuffAddToSP<BuffTypes::团队Leader>,
		BuffAddToSP<BuffTypes::sudo_rm_rf>,
		BuffAddToSP<BuffTypes::裁员大动脉>,
		BuffAddToSP<BuffTypes::筋肉兄贵>,
		BuffAddToSP<BuffTypes::投资>,
		BuffAddToSP<BuffTypes::劳动法>,
	};

	struct BuffContainer {
		static constexpr int32_t numBuffers{ (int32_t)BuffTypes::__MaxValue__ };

		std::array<int32_t, numBuffers> nums;
		std::array<int8_t, numBuffers> flags;
		xx::Listi32<std::pair<BuffTypes, int32_t>> logs;
		StatPanel sp;

		void Init() {
			memset(&nums, 0, sizeof(nums));
			memset(&flags, 0, sizeof(flags));
			logs.Clear();
			sp.Clear();
		}

		int32_t& NumOf(BuffTypes bt) const {
			return (int32_t&)nums[(int32_t)bt];
		}

		int8_t& FlagOf(BuffTypes bt) const {
			return (int8_t&)flags[(int32_t)bt];
		}

		std::pair<int32_t, int8_t> At(BuffTypes bt) const {
			return { nums[(int32_t)bt], flags[(int32_t)bt] };
		}

		// for assert
		bool IsLimited(BuffTypes bt) const {
			assert(nums[(int32_t)bt] <= BuffLimits[(int32_t)bt]);
			return nums[(int32_t)bt] == BuffLimits[(int32_t)bt];
		}

		bool TryAdd(BuffTypes bt) {
			auto i = (int32_t)bt;
			if (nums[i] < BuffLimits[i]) {
				++nums[i];
				BuffAddToSPs[i](sp);
				logs.Emplace(bt, 1);
				return true;
			}
			return false;
		}

		void NewRound() {
			memset(&flags, 0, sizeof(flags));
			// todo: cleanup some round sum values like "肉食主义" "技术债" ...
		}

		// todo: get drop list with exclude limited ?

		// todo: get buff info for UI display
	};

}
