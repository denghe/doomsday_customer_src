#pragma once

namespace Game {

	struct BuffRichLabelFiller {
		static constexpr float cFieldNameWidth{ 250 };
		static constexpr float cValueWidth{ 70 };
		// todo: more cfg

		xx::RichLabel* rl{};
		BuffTypes bt{};
		std::u32string name, desc;

		std::optional<Stat_t> healthPoint;
		std::optional<Stat_t> healthRatio;
		std::optional<Stat_t> healthPointMax;
		std::optional<Stat_t> healthRegeneration;
		std::optional<Stat_t> defensePoint;
		std::optional<Stat_t> defenseRatio;
		std::optional<Stat_t> dodgePoint;
		std::optional<Stat_t> dodgeRatio;
		std::optional<Stat_t> dodgeChance;
		std::optional<Stat_t> movementSpeedPoint;
		std::optional<Stat_t> movementSpeed;
		std::optional<Stat_t> movementSpeedPerFrame;
		std::optional<Stat_t> damageRatio;
		std::optional<Stat_t> criticalChance;
		std::optional<Stat_t> criticalBonusRatio;
		std::optional<Stat_t> attackSpeed;
		std::optional<Stat_t> attackSpeedRatio;
		std::optional<Stat_t> luckyPoint;
		std::optional<Stat_t> harvestRatio;
		std::optional<Stat_t> fieldOfVision;

		BuffRichLabelFiller(xx::Node* parent_) {
			auto&& rl_ = parent_->MakeChildren<xx::RichLabel>();
			rl_->Init(parent_->z + 1, {}, { 1,1 }, {}, cFieldNameWidth + cValueWidth);
			rl = rl_.pointer;
		}

		XX_INLINE BuffRichLabelFiller& Set_BuffType(BuffTypes bt_) { bt = bt_; return *this; }
		XX_INLINE BuffRichLabelFiller& Set_Name(std::u32string name_) { name = std::move(name_); return *this; }
		XX_INLINE BuffRichLabelFiller& Set_Desc(std::u32string desc_) { desc = std::move(desc_); return *this; }

		XX_INLINE BuffRichLabelFiller& Set_HealthPoint(Stat_t v) { healthPoint = v; return *this; }
		XX_INLINE BuffRichLabelFiller& Set_HealthRatio(Stat_t v) { healthRatio = v; return *this; }
		XX_INLINE BuffRichLabelFiller& Set_HealthPointMax(Stat_t v) { healthPointMax = v; return *this; }
		XX_INLINE BuffRichLabelFiller& Set_HealthRegeneration(Stat_t v) { healthRegeneration = v; return *this; }
		XX_INLINE BuffRichLabelFiller& Set_DefensePoint(Stat_t v) { defensePoint = v; return *this; }
		XX_INLINE BuffRichLabelFiller& Set_DefenseRatio(Stat_t v) { defenseRatio = v; return *this; }
		XX_INLINE BuffRichLabelFiller& Set_DodgePoint(Stat_t v) { dodgePoint = v; return *this; }
		XX_INLINE BuffRichLabelFiller& Set_DodgeRatio(Stat_t v) { dodgeRatio = v; return *this; }
		XX_INLINE BuffRichLabelFiller& Set_DodgeChance(Stat_t v) { dodgeChance = v; return *this; }
		XX_INLINE BuffRichLabelFiller& Set_MovementSpeedPoint(Stat_t v) { movementSpeedPoint = v; return *this; }
		XX_INLINE BuffRichLabelFiller& Set_MovementSpeed(Stat_t v) { movementSpeed = v; return *this; }
		XX_INLINE BuffRichLabelFiller& Set_MovementSpeedPerFrame(Stat_t v) { movementSpeedPerFrame = v; return *this; }
		XX_INLINE BuffRichLabelFiller& Set_DamageRatio(Stat_t v) { damageRatio = v; return *this; }
		XX_INLINE BuffRichLabelFiller& Set_CriticalChance(Stat_t v) { criticalChance = v; return *this; }
		XX_INLINE BuffRichLabelFiller& Set_CriticalBonusRatio(Stat_t v) { criticalBonusRatio = v; return *this; }
		XX_INLINE BuffRichLabelFiller& Set_AttackSpeed(Stat_t v) { attackSpeed = v; return *this; }
		XX_INLINE BuffRichLabelFiller& Set_AttackSpeedRatio(Stat_t v) { attackSpeedRatio = v; return *this; }
		XX_INLINE BuffRichLabelFiller& Set_LuckyPoint(Stat_t v) { luckyPoint = v; return *this; }
		XX_INLINE BuffRichLabelFiller& Set_HarvestRatio(Stat_t v) { harvestRatio = v; return *this; }
		XX_INLINE BuffRichLabelFiller& Set_FieldOfVision(Stat_t v) { fieldOfVision = v; return *this; }

		template<bool isPercentage = false>
		XX_INLINE void TryAppend(std::u32string_view fieldName, std::optional<Stat_t>& v) {
			if (v.has_value()) {
				// todo: posstive set color blue? negative set color red?
				rl->AddLimitedWidthText(fieldName, cFieldNameWidth);

				std::string s;
				if constexpr (isPercentage) {
					s = xx::ToString(*v * 100);
					s.push_back(U'%');
				}
				else {
					s = xx::ToString(*v);
				}

				auto c = xx::RGBA8_Green;
				if (*v < 0) {
					c = xx::RGBA8_Red;
				}
				rl->AddRightText(s, 1, c);

				rl->AddText(U"\n");
			}
		}

		inline xx::RichLabel* Commit() {
			// todo: fill icon
			rl->AddLimitedWidthText(name, cFieldNameWidth + cValueWidth - 1, 2).AddText(U"\n");
			TryAppend(U"基础血量", healthPoint);
			TryAppend<true>(U"血量倍率", healthRatio);
			TryAppend(U"最大血量", healthPointMax);
			TryAppend(U"血量回复", healthRegeneration);
			TryAppend(U"防御值", defensePoint);
			TryAppend<true>(U"防御倍率", defenseRatio);
			TryAppend(U"闪避值", dodgePoint);
			TryAppend<true>(U"闪避倍率", dodgeRatio);
			TryAppend<true>(U"闪避率", dodgeChance);
			TryAppend(U"移动值", movementSpeedPoint);
			TryAppend(U"移动速度", movementSpeed);
			TryAppend(U"帧移动速度", movementSpeedPerFrame);
			TryAppend<true>(U"伤害倍率", damageRatio);
			TryAppend<true>(U"暴击概率", criticalChance);
			TryAppend<true>(U"暴击倍率", criticalBonusRatio);
			TryAppend(U"攻速", attackSpeed);
			TryAppend<true>(U"攻速倍率", attackSpeedRatio);
			TryAppend(U"幸运值", luckyPoint);
			TryAppend(U"收获值", harvestRatio);
			TryAppend(U"视野", fieldOfVision);
			// todo: analysis color control char?
			rl->AddText(desc);
			rl->Commit();
			return rl;
		}
	};


	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::__Default__>(xx::Node* parent_) {
		assert(false);
		return {};
	}

	// 攻击+20%，攻速-5%，最大血量-5%
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::_996>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::_996)
			.Set_Name(U"996")
			.Set_Desc(U"996's desc")
			.Set_DamageRatio(0.2f)
			.Set_AttackSpeedRatio(-0.05f)
			.Set_HealthRatio(-0.05f)
			.Commit();
	}

	// 血量回复+10/s
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::中药调理>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::中药调理)
			.Set_Name(U"中药调理")
			.Set_Desc(U"中药调理's desc")
			.Set_HealthRegeneration(10)
			.Commit();
	}

	// 最大血量+15%，下一回合以30%血量起步
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::跑步机>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::跑步机)
			.Set_Name(U"跑步机")
			.Set_Desc(U"下一回合以30%血量起步")
			.Set_HealthRatio(0.15)
			.Commit();
	}

	// 范围+10，幸运-5
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::眼镜>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::眼镜)
			.Set_Name(U"眼镜")
			.Set_Desc(U"眼镜's desc")
			.Set_FieldOfVision(10)
			.Set_LuckyPoint(-5)
			.Commit();
	}

	// 射速+5%
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::肘击>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::肘击)
			.Set_Name(U"肘击")
			.Set_Desc(U"肘击's desc")
			.Set_AttackSpeedRatio(0.05)
			.Commit();
	}

	// 攻击+15%，血量回复-5/s
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::加班餐>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::加班餐)
			.Set_Name(U"加班餐")
			.Set_Desc(U"加班餐's desc")
			.Set_DamageRatio(0.15)
			.Set_HealthRegeneration(-5)
			.Commit();
	}

	// 移速+1
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::冲刺鞋子>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::冲刺鞋子)
			.Set_Name(U"冲刺鞋子")
			.Set_Desc(U"冲刺鞋子's desc")
			.Set_MovementSpeedPoint(1)
			.Commit();
	}

	// 攻速+20%，移速-1
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::旋转办公椅>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::旋转办公椅)
			.Set_Name(U"旋转办公椅")
			.Set_Desc(U"旋转办公椅's desc")
			.Set_AttackSpeedRatio(0.2)
			.Set_MovementSpeedPoint(-1)
			.Commit();
	}

	// 攻速+10%，幸运-5，收获+5%
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::塑料友情>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::塑料友情)
			.Set_Name(U"塑料友情")
			.Set_Desc(U"塑料友情's desc")
			.Set_AttackSpeedRatio(0.1)
			.Set_LuckyPoint(-5)
			.Set_HarvestRatio(0.05)
			.Commit();
	}

	// 护甲+5
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::钢板>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::钢板)
			.Set_Name(U"钢板")
			.Set_Desc(U"钢板's desc")
			.Set_DefensePoint(5)
			.Commit();
	}

	// 最大血量+5%，血量回复+10，伤害-5%
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::海底捞>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::海底捞)
			.Set_Name(U"海底捞")
			.Set_Desc(U"海底捞's desc")
			.Set_HealthRatio(0.05)
			.Set_HealthRegeneration(10)
			.Set_DamageRatio(-0.05)
			.Commit();
	}

	// 暴击伤害+15%
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::义愤填膺>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::义愤填膺)
			.Set_Name(U"义愤填膺")
			.Set_Desc(U"义愤填膺's desc")
			.Set_CriticalBonusRatio(0.15)
			.Commit();
	}

	// 杀死小怪时有70%概率+30HP，30%概率-30HP
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::国潮外卖>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::国潮外卖)
			.Set_Name(U"国潮外卖")
			.Set_Desc(U"杀死小怪时有70%概率+30HP，30%概率-30HP")
			.Commit();
	}

	// 攻击-20%，幸运+30，血量回复+15/s
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::躺平>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::躺平)
			.Set_Name(U"躺平")
			.Set_Desc(U"躺平's desc")
			.Set_DamageRatio(-0.2)
			.Set_LuckyPoint(30)
			.Set_HealthRegeneration(15)
			.Commit();
	}

	// 攻击+30%，攻速+10%，每5秒不受控制的固定向前方冲刺0.5秒
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::社交牛逼症>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::社交牛逼症)
			.Set_Name(U"社交牛逼症")
			.Set_Desc(U"每5秒不受控制的固定向前方冲刺0.5秒")
			.Set_DamageRatio(0.3)
			.Set_AttackSpeedRatio(0.1)
			.Commit();
	}

	// 幸运+40，攻击-5%
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::风水大师>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::风水大师)
			.Set_Name(U"风水大师")
			.Set_Desc(U"风水大师's desc")
			.Set_LuckyPoint(40)
			.Set_DamageRatio(-0.05)
			.Commit();
	}

	// 攻击-5%，攻速+30%
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::CRUD糕手>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::CRUD糕手)
			.Set_Name(U"CRUD糕手")
			.Set_Desc(U"CRUD糕手's desc")
			.Set_DamageRatio(-0.05)
			.Set_AttackSpeedRatio(0.3)
			.Commit();
	}

	// 幸运+20，移速-1，血量回复+15/s
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::动漫达人>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::动漫达人)
			.Set_Name(U"动漫达人")
			.Set_Desc(U"动漫达人's desc")
			.Set_LuckyPoint(20)
			.Set_MovementSpeedPoint(-1)
			.Set_HealthRegeneration(15)
			.Commit();
	}

	// 攻击+15%，攻速+30%，血量回复-20/s
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::屎山代码>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::屎山代码)
			.Set_Name(U"屎山代码")
			.Set_Desc(U"屎山代码's desc")
			.Set_DamageRatio(0.15)
			.Set_AttackSpeedRatio(0.3)
			.Set_HealthRegeneration(-20)
			.Commit();
	}

	// 最大血量-50%，攻速+100%，攻击-20%
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::福报>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::福报)
			.Set_Name(U"福报")
			.Set_Desc(U"福报's desc")
			.Set_HealthRatio(-0.5)
			.Set_AttackSpeedRatio(1)
			.Set_DamageRatio(-0.2)
			.Commit();
	}

	// 移速+2，闪避+3%
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::归宅部>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::归宅部)
			.Set_Name(U"归宅部")
			.Set_Desc(U"归宅部's desc")
			.Set_MovementSpeedPoint(2)
			.Set_DodgeRatio(0.03)
			.Commit();
	}

	// 攻击+3%，幸运+5，血量回复+5/s，攻速+5%，最大血量+5%，暴击率+3%，爆伤+5%
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::为什么我不是沪爷>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::为什么我不是沪爷)
			.Set_Name(U"为什么我不是沪爷")
			.Set_Desc(U"为什么我不是沪爷's desc")
			.Set_DamageRatio(0.03)
			.Set_LuckyPoint(5)
			.Set_HealthRegeneration(5)
			.Set_AttackSpeedRatio(0.05)
			.Set_HealthRatio(0.05)
			.Set_CriticalChance(0.03)
			.Set_CriticalBonusRatio(0.05)
			.Commit();
	}

	// 幸运 +60，攻击 -10%，血量回复 -5/s
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::彩票研究者>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::彩票研究者)
			.Set_Name(U"彩票研究者")
			.Set_Desc(U"彩票研究者's desc")
			.Set_LuckyPoint(60)
			.Set_DamageRatio(-0.1)
			.Set_HealthRegeneration(-5)
			.Commit();
	}

	// 闪避+15%，攻速 -10%，每3秒强制向后方冲刺0.5秒
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::社恐>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::社恐)
			.Set_Name(U"社恐")
			.Set_Desc(U"每3秒强制向后方冲刺0.5秒")
			.Set_DodgeRatio(0.15)
			.Set_AttackSpeedRatio(-0.1)
			.Commit();
	}

	// 杀死小怪时攻击+3%（一局内）
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::肉食主义>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::肉食主义)
			.Set_Name(U"肉食主义")
			.Set_Desc(U"杀死小怪时攻击+3%（一局内）")
			.Commit();
	}

	// 按下特殊键F进入10秒无敌状态，但是不能行动。一局一次
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::请假>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::请假)
			.Set_Name(U"请假")
			.Set_Desc(U"按下特殊键F进入10秒无敌状态，但是不能行动。一局一次")
			.Commit();
	}

	// 攻击+40%，攻速+20%，每回合最大HP-10%，收获-10%
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::技术债>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::技术债)
			.Set_Name(U"技术债")
			.Set_Desc(U"每回合最大HP-10%，收获-10%")
			.Set_DamageRatio(0.4)
			.Set_AttackSpeedRatio(0.2)
			.Commit();
	}

	// 攻击射出的子弹额外附加30%幸运值倍率的伤害
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::南无加特林机枪菩萨>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::南无加特林机枪菩萨)
			.Set_Name(U"南无加特林机枪菩萨")
			.Set_Desc(U"攻击射出的子弹额外附加30%幸运值倍率的伤害")
			.Commit();
	}

	// 最大血量提升，提升值为300%幸运值
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::南无阿弥陀佛>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::南无阿弥陀佛)
			.Set_Name(U"南无阿弥陀佛")
			.Set_Desc(U"最大血量提升，提升值为300%幸运值")
			.Commit();
	}

	// 攻击-3%，幸运-5，血量回复-5/s，攻速-5%，最大血量-5%，暴击率-3%，爆伤-5%，收获+200
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::沪爷>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::沪爷)
			.Set_Name(U"沪爷")
			.Set_Desc(U"沪爷's desc")
			.Set_DamageRatio(-0.03)
			.Set_LuckyPoint(-5)
			.Set_HealthRegeneration(-5)
			.Set_AttackSpeedRatio(-0.05)
			.Set_HealthRatio(-0.05)
			.Set_CriticalChance(-0.03)
			.Set_CriticalBonusRatio(-0.05)
			.Set_HarvestRatio(2)
			.Commit();
	}

	// 按F生成一个好兄弟，好兄弟会进入15秒无敌状态，和玩家拥有相同的攻击力，怪物会优先攻击好兄弟。15秒后结束。一局一次
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::坐在后面的好兄弟>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::坐在后面的好兄弟)
			.Set_Name(U"坐在后面的好兄弟")
			.Set_Desc(U"按F生成一个好兄弟，好兄弟会进入15秒无敌状态，和玩家拥有相同的攻击力，怪物会优先攻击好兄弟。15秒后结束。一局一次")
			.Commit();
	}

	// 生成3个好兄弟，拥有玩家50%攻击力和100%血量。自身攻击-25%，移速-1
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::团队Leader>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::团队Leader)
			.Set_Name(U"团队Leader")
			.Set_Desc(U"生成3个好兄弟，拥有玩家50%攻击力和100%血量")
			.Set_DamageRatio(-0.25)
			.Set_MovementSpeedPoint(-1)
			.Commit();
	}

	// 移速+5，护甲+30，攻击-100%
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::sudo_rm_rf>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::sudo_rm_rf)
			.Set_Name(U"sudo_rm_rf")
			.Set_Desc(U"sudo_rm_rf's desc")
			.Set_MovementSpeedPoint(5)
			.Set_DefensePoint(30)
			.Set_DamageRatio(-1)
			.Commit();
	}

	// 死亡时回复20%HP保证不死，一局一次。触发时对全体怪物造成100%幸运值+300%倍率的伤害。
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::裁员大动脉>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::裁员大动脉)
			.Set_Name(U"裁员大动脉")
			.Set_Desc(U"死亡时回复20%HP保证不死，一局一次。触发时对全体怪物造成100%幸运值+300%倍率的伤害。")
			.Commit();
	}

	// 最大血量+50%，伤害+50%，下一回合以1%血量起步
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::筋肉兄贵>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::筋肉兄贵)
			.Set_Name(U"筋肉兄贵")
			.Set_Desc(U"下一回合以1%血量起步")
			.Set_HealthRatio(0.5)
			.Set_DamageRatio(0.5)
			.Commit();
	}

	// 每回合结束时-50%~+100%代币
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::投资>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::投资)
			.Set_Name(U"投资")
			.Set_Desc(U"每回合结束时-50%~+100%代币")
			.Commit();
	}

	// 无作用
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::劳动法>(xx::Node* parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::劳动法)
			.Set_Name(U"劳动法")
			.Set_Desc(U"无作用")
			.Commit();
	}





	inline void UI_BuffBag::Init(Stage* stage_, int z_, XY const& position_, XY const& scale_, XY const& anchor_, XY const& size_) {
		this->ScrollView::Init(z_ + 1, position_, scale_, anchor_, size_, 1);
		MakeChildren<xx::Scale9Sprite>()->Init(z_, -cMargin, { 1,1 }, {}, size + cMargin * 2, gLooper.btnCfg1);
		content = MakeContent<UI_BuffsContent>()->Init(stage_, this);
	}




	inline UI_BuffsContent* UI_BuffsContent::Init(Stage* stage_, UI_BuffBag* buffBag_) {
		stage = stage_;
		buffBag = buffBag_;
		Node::Init(buffBag_->z + 1, {}, { 1,1 }, {}, buffBag_->size);

		UpdateSize();
		return this;
	}

	inline void UI_BuffsContent::UpdateSize() {
		auto& items = stage->player->buffs.logs;
		// calc bag grid size
		numCols = int32_t(buffBag->size.x + cItemMargin.x) / int32_t(cItemSize.x + cItemMargin.x);
		itemMarginX = (buffBag->size.x - cItemSize.x * numCols) / (numCols - 1);
		numRows = (int32_t)items.size() / numCols;
		if (numRows * numCols < items.size()) ++numRows;
		size = { numCols * cItemSize.x + (numCols - 1) * itemMarginX, numRows * cItemSize.y + (numRows - 1) * cItemMargin.y };
		buffBag->InitContentSize<false>(size); 
	}

	inline BuffTypes* UI_BuffsContent::TryGetMousePosItem() {
		if (buffBag->MousePosInArea()) {
			auto& items = stage->player->buffs.logs;
#if 1
			auto rowIdxBegin = int32_t(size.y + parent->position.y - buffBag->size.y) / int32_t(cItemSize.y + cItemMargin.y);
			auto rowIdxEnd = rowIdxBegin + int32_t(buffBag->size.y) / int32_t(cItemSize.y + cItemMargin.y) + 1;
			auto basePos = worldMinXY;
			auto mp = xx::gEngine->mouse.pos;
			for (int32_t rowIdx = rowIdxBegin; rowIdx <= rowIdxEnd; ++rowIdx) {
				for (int32_t colIdx = 0; colIdx < numCols; ++colIdx) {
					auto itemIndex = rowIdx * numCols + colIdx;
					if (itemIndex >= items.size()) return {};
					auto pos = basePos + XY{ colIdx * (cItemSize.x + itemMarginX) + cItemSize.x / 2, size.y - rowIdx * (cItemSize.y + cItemMargin.y) - cItemSize.y / 2 };
					xx::FromTo<XY> aabb{ pos - cItemSize / 2, pos + cItemSize / 2 };
					if (mp.x < aabb.from.x || aabb.to.x < mp.x || mp.y < aabb.from.y || aabb.to.y < mp.y) continue;
					return &items[itemIndex];
				}
			}
#else
			// todo: optimize
#endif
		}
		return {};
	}

	inline void UI_BuffsContent::Draw() {
		auto& items = stage->player->buffs.logs;
		xx::Quad q;
		q.SetScale(cScale);
		// calculate row cut range
		auto rowIdxBegin = int32_t(size.y + parent->position.y - buffBag->size.y) / int32_t(cItemSize.y + cItemMargin.y);
		auto rowIdxEnd = rowIdxBegin + int32_t(buffBag->size.y) / int32_t(cItemSize.y + cItemMargin.y) + 1;
		auto basePos = worldMinXY;
		for (int32_t rowIdx = rowIdxBegin; rowIdx <= rowIdxEnd; ++rowIdx) {
			for (int32_t colIdx = 0; colIdx < numCols; ++colIdx) {
				auto itemIndex = rowIdx * numCols + colIdx;
				if (itemIndex >= items.size()) return;
				auto pos = basePos + XY{ colIdx * (cItemSize.x + itemMarginX) + cItemSize.x / 2, size.y - rowIdx * (cItemSize.y + cItemMargin.y) - cItemSize.y / 2 };
				auto& frame = gLooper.res.buff_[(int32_t)items[itemIndex]];
				q.SetFrame(frame).SetPosition(pos).Draw();
			}
		}
	}



	inline void UI_BuffInfo::Init() {
		this->Node::Init();
		MakeChildren<xx::Scale9Sprite>()->Init(1, -cInfoMargin, { 1,1 }, {}, 100, gLooper.btnCfg);
	}

	inline void UI_BuffInfo::Set(BuffTypes bt, XY pos) {
		if (children.len > 1) {
			children.PopBack();
		}
		position = pos;
		FillTransRecursive();
		auto rl = MakeBuffRichLabels[(int32_t)bt](this);
		auto bg = (xx::Scale9Sprite*)children[0].pointer;
		bg->size = { BuffRichLabelFiller::cFieldNameWidth + BuffRichLabelFiller::cValueWidth + cInfoMargin.x * 2, rl->size.y + cInfoMargin.y * 2 };
	}




	
	inline void UI_BuffShopGoodsItem::Init(Stage* stage_, XY pos, BuffTypes bt) {
		stage = stage_;
	}


	inline void UI_BuffShopGoodsList::Init(Stage* stage_) {
		stage = stage_;
	}

	inline void UI_BuffShopGoodsList::Refresh() {
		children.Clear();
		// todo: getlist from player buffs
		// make UI_BuffShopGoodsItem

		//static constexpr XY goodsBtnSize{ 300, 450 };
		//static constexpr float goodsNameWidth{ 150 };
		//static constexpr float goodsBtnMargin{ 20 };
		//static constexpr XY goodsBtnOffset{ leftPos + (goodsBtnSize.x * 3 + goodsBtnMargin * 2) / 2, 0 };
		//static constexpr XY goodsBtnPoss[3] = { { goodsBtnOffset + XY{ -goodsBtnSize.x * 1.f - goodsBtnMargin, 0 } }, { goodsBtnOffset + XY{ 0, 0 } }, { goodsBtnOffset + XY{ goodsBtnSize.x * 1.f + goodsBtnMargin, 0 } } };
		//static constexpr XY goodsBtnContentPoss[3] = { goodsBtnPoss[0] + XY{ 0, 200 }, goodsBtnPoss[1] + XY{ 0, 200 }, goodsBtnPoss[2] + XY{ 0, 200 } };


		//MakeChildren<xx::EmptyButton>()->Init(3, goodsBtnPoss[0], 0.5f, gLooper.btnCfg4, goodsBtnSize);
		//MakeChildren<xx::EmptyButton>()->Init(3, goodsBtnPoss[1], 0.5f, gLooper.btnCfg4, goodsBtnSize);
		//MakeChildren<xx::EmptyButton>()->Init(3, goodsBtnPoss[2], 0.5f, gLooper.btnCfg4, goodsBtnSize);

		//static constexpr float goodsBtnContentWidth{ goodsBtnSize.x - goodsBtnMargin * 2 };
		//auto goodsBtnIconFrame = gLooper.res.ui_money;
		//static constexpr float goodsBtnIconDrawHeight{ 100 };
		//auto goodsBtnIconScale = goodsBtnIconDrawHeight / goodsBtnIconFrame->spriteSize.y;
		//auto goodsBtnIconDrawWidth = goodsBtnIconFrame->spriteSize.x * goodsBtnIconScale;
		//{
		//	// goods1
		//	auto& rich = MakeChildren<xx::RichLabel>()->Init(4, goodsBtnContentPoss[0], 1, { 0.5f, 1 }, goodsBtnContentWidth);
		//	rich.SetOffset(goodsBtnContentWidth / 2 - goodsBtnIconDrawWidth / 2).AddPicture(gLooper.res.ui_money, goodsBtnIconScale)
		//		.AddLimitedWidthText(U"\n攻击倍率", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"+20%", 2, xx::RGBA8_Red)
		//		.AddLimitedWidthText(U"\n每秒回血", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"-5%", 2, xx::RGBA8_Green)
		//		.AddLimitedWidthText(U"\n最大血量", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"-5%", 2, xx::RGBA8_Green)
		//		.Commit();
		//}
		//{
		//	// goods2
		//	auto& rich = MakeChildren<xx::RichLabel>()->Init(4, goodsBtnContentPoss[1], 1, { 0.5f, 1 }, goodsBtnContentWidth);
		//	rich.SetOffset(goodsBtnContentWidth / 2 - goodsBtnIconDrawWidth / 2).AddPicture(gLooper.res.ui_money, goodsBtnIconScale)
		//		.AddLimitedWidthText(U"\n攻击倍率", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"+20%", 2, xx::RGBA8_Red)
		//		.AddLimitedWidthText(U"\n每秒回血", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"-5%", 2, xx::RGBA8_Green)
		//		.AddLimitedWidthText(U"\n最大血量", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"-5%", 2, xx::RGBA8_Green)
		//		.Commit();
		//}
		//{
		//	// goods3
		//	auto& rich = MakeChildren<xx::RichLabel>()->Init(4, goodsBtnContentPoss[2], 1, { 0.5f, 1 }, goodsBtnContentWidth);
		//	rich.SetOffset(goodsBtnContentWidth / 2 - goodsBtnIconDrawWidth / 2).AddPicture(gLooper.res.ui_money, goodsBtnIconScale)
		//		.AddLimitedWidthText(U"\n攻击倍率", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"+20%", 2, xx::RGBA8_Red)
		//		.AddLimitedWidthText(U"\n每秒回血", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"-5%", 2, xx::RGBA8_Green)
		//		.AddLimitedWidthText(U"\n最大血量", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"-5%", 2, xx::RGBA8_Green)
		//		.Commit();
		//}

	}


	inline void UI_PlayerStat::Init(Stage* stage_) {
		stage = stage_;
	}

	inline void UI_PlayerStat::Refresh() {
		children.Clear();
		// todo

	}


	inline void UI_BuffShop::Init(Stage* stage_) {
		Node::Init(1);
		stage = stage_;

		static constexpr float leftPos{ -630 };
		static constexpr float rightPos{ 630 };
		static constexpr float topPos{ 440 };
		static constexpr float bottomPos{ -440 };
		static constexpr auto bgScale = 1080.f * 0.95f / gLooper.res._size_ui_paper.x;
		static constexpr float buffBagHeight{ 200 };
		static constexpr XY moneyIconSize{ 50, 50 };
		static constexpr float moneyIconTextMargin{ 30 };
		static constexpr float moneyIconScale{ moneyIconSize.x / gLooper.res._size_ui_money.x };
		static constexpr XY moneyIconPos{ leftPos + moneyIconSize.x, topPos - moneyIconSize.y / 2 };
		static constexpr XY moneyTextPos{ moneyIconPos.x + moneyIconTextMargin, topPos - moneyIconSize.y / 2 };

		// avoid user click other buttons
		MakeChildren<xx::SwallowButton>()->Init(1).onClicked = [] {};

		// bg paper
		MakeChildren<xx::Image>()->Init(2, {}, bgScale, 0.5f, gLooper.res.ui_paper, xx::ImageRadians::PiDiv2);

		// player coin
		MakeChildren<xx::Image>()->Init(2, moneyIconPos, moneyIconScale, { 1, 0.5f }, gLooper.res.ui_money);
		MakeChildren<xx::Label>()->Init(3, moneyTextPos, 2.f, { 0, 0.5f }, xx::RGBA8_Black, U"123456");

		// shop title
		MakeChildren<xx::Label>()->Init(3, { 0, topPos }, 4.f, { 0.5f, 1 }, xx::RGBA8_Black, U"商店");

		// button: refresh goodsList
		MakeChildren<xx::IconButton>()->Init(3, { rightPos, topPos }, 1, gLooper.btnCfg3i, gLooper.res.ui_refresh, U"$123", {}, xx::RGBA8_Black).onClicked = [&]() {
			// todo
		};

		// goodsList list
		goodsList = MakeChildren<UI_BuffShopGoodsList>();
		goodsList->Init(stage_);

		// player stat panel
		playerStat = MakeChildren<UI_PlayerStat>();
		playerStat->Init(stage_);

		// button: next round
		auto& btnNextRound = MakeChildren<xx::Button>()->Init(3, { rightPos, bottomPos }, { 1, 0 }, gLooper.btnCfg3, U"下一关");
		btnNextRound.onClicked = [this] {
			// todo
		};

		// player buff bag
		XY buffBagSize{ rightPos - leftPos - btnNextRound.size.x - moneyIconTextMargin - UI_BuffBag::cMargin.x * 2, buffBagHeight };
		XY buffBagPos{ leftPos + UI_BuffBag::cMargin.x, bottomPos + UI_BuffBag::cMargin.y };
		buffBag = MakeChildren<UI_BuffBag>();
		buffBag->Init(stage_, 3, buffBagPos, 1, 0, buffBagSize);
	}





	inline void Test5::Init() {
		ui.Emplace()->Init();

		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy7m + XY{ 10, -10 }
			, Cfg::xy7a, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
		});

		gridSize = { 60, 60 };
		mapSize = 128 * gridSize;

		ground.Emplace()->Init(this, mapSize, gLooper.res.ground_cell2);
		player.Emplace<Player_1>()->Init(this);

		// fill player buff for test
		for (int32_t i = 1; i < (int32_t)BuffTypes::__MaxValue__; ++i) {
			player->buffs.logs.emplace_back((BuffTypes)i);
		}

		buffInfo.Emplace()->Init();

		buffShop.Emplace()->Init(this);


		camera.scale = 1.f;
		camera.mapSize = mapSize;
		camera.newOriginal = camera.original = mapSize * 0.5f;
	}

	inline void Test5::Update() {
		// scale control
		if (gLooper.KeyDownDelay(xx::KeyboardKeys::Z, 0.02f)) {
			camera.IncreaseScale(0.01f, 5);
		}
		else if (gLooper.KeyDownDelay(xx::KeyboardKeys::X, 0.02f)) {
			camera.DecreaseScale(0.01f, 0.1f);
		}

		player->Update();
		camera.SetOriginal<true>(player->pos, camera.ToLogicPos(gLooper.mouse.pos));
		camera.Update();



		++time;
	}

	inline void Test5::Draw() {
		// draw floor
		ground->Draw();

		// calculate display cut area
		auto areaMin = camera.ToLogicPos({ -gLooper.width_2 - Cfg::unitSize * 2, gLooper.height_2 + Cfg::unitSize * 2 });
		auto areaMax = camera.ToLogicPos({ gLooper.width_2 + Cfg::unitSize * 2, -gLooper.height_2 - Cfg::unitSize * 2 });

		// draw game items ( order by y )
		// 
		// prepare
		auto& yd = gLooper.yDraws;

		yd.Emplace(player->pos.y, player.pointer);

		//for (auto e = cgs.len, i = 0; i < e; ++i) {
		//	auto& o = cgs[i];
		//	if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
		//	yd.Emplace(o->pos.y, o.pointer);
		//}

		// sort
		std::sort(yd.buf, yd.buf + yd.len, [](auto& a, auto& b) { return a.first < b.first; });

		// draw
		for (auto e = yd.len, i = 0; i < e; ++i) {
			yd[i].second->Draw();
		}

		// clean up
		yd.Clear();


		if (buffShop) {
			gLooper.DrawNode(buffShop);
		}

		if (buffShop) {
			if (auto bt = buffShop->buffBag->content->TryGetMousePosItem()) {
				buffInfo->Set(*bt, gLooper.mouse.pos);
				gLooper.DrawNode(buffInfo);
			}
		}

		gLooper.DrawNode(ui);
	}
}
