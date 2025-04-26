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

		BuffRichLabelFiller(xx::Shared<xx::Node>& parent_) {
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


	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::__Default__>(xx::Shared<xx::Node>& parent_) {
		assert(false);
		return {};
	}

	// 攻击+20%，攻速-5%，最大血量-5%
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::_996>(xx::Shared<xx::Node>& parent_) {
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
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::中药调理>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::中药调理)
			.Set_Name(U"中药调理")
			.Set_Desc(U"中药调理's desc")
			.Set_HealthRegeneration(10)
			.Commit();
	}

	// 最大血量+15%，下一回合以30%血量起步
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::跑步机>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::跑步机)
			.Set_Name(U"跑步机")
			.Set_Desc(U"下一回合以30%血量起步")
			.Set_HealthRatio(0.15)
			.Commit();
	}

	// 范围+10，幸运-5
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::眼镜>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::眼镜)
			.Set_Name(U"眼镜")
			.Set_Desc(U"眼镜's desc")
			.Set_FieldOfVision(10)
			.Set_LuckyPoint(-5)
			.Commit();
	}

	// 射速+5%
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::肘击>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::肘击)
			.Set_Name(U"肘击")
			.Set_Desc(U"肘击's desc")
			.Set_AttackSpeedRatio(0.05)
			.Commit();
	}

	// 攻击+15%，血量回复-5/s
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::加班餐>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::加班餐)
			.Set_Name(U"加班餐")
			.Set_Desc(U"加班餐's desc")
			.Set_DamageRatio(0.15)
			.Set_HealthRegeneration(-5)
			.Commit();
	}

	// 移速+1
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::冲刺鞋子>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::冲刺鞋子)
			.Set_Name(U"冲刺鞋子")
			.Set_Desc(U"冲刺鞋子's desc")
			.Set_MovementSpeedPoint(1)
			.Commit();
	}

	// 攻速+20%，移速-1
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::旋转办公椅>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::旋转办公椅)
			.Set_Name(U"旋转办公椅")
			.Set_Desc(U"旋转办公椅's desc")
			.Set_AttackSpeedRatio(0.2)
			.Set_MovementSpeedPoint(-1)
			.Commit();
	}

	// 攻速+10%，幸运-5，收获+5%
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::塑料友情>(xx::Shared<xx::Node>& parent_) {
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
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::钢板>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::钢板)
			.Set_Name(U"钢板")
			.Set_Desc(U"钢板's desc")
			.Set_DefensePoint(5)
			.Commit();
	}

	// 最大血量+5%，血量回复+10，伤害-5%
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::海底捞>(xx::Shared<xx::Node>& parent_) {
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
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::义愤填膺>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::义愤填膺)
			.Set_Name(U"义愤填膺")
			.Set_Desc(U"义愤填膺's desc")
			.Set_CriticalBonusRatio(0.15)
			.Commit();
	}

	// 杀死小怪时有70%概率+30HP，30%概率-30HP
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::国潮外卖>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::国潮外卖)
			.Set_Name(U"国潮外卖")
			.Set_Desc(U"杀死小怪时有70%概率+30HP，30%概率-30HP")
			.Commit();
	}

	// 攻击-20%，幸运+30，血量回复+15/s
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::躺平>(xx::Shared<xx::Node>& parent_) {
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
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::社交牛逼症>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::社交牛逼症)
			.Set_Name(U"社交牛逼症")
			.Set_Desc(U"每5秒不受控制的固定向前方冲刺0.5秒")
			.Set_DamageRatio(0.3)
			.Set_AttackSpeedRatio(0.1)
			.Commit();
	}

	// 幸运+40，攻击-5%
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::风水大师>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::风水大师)
			.Set_Name(U"风水大师")
			.Set_Desc(U"风水大师's desc")
			.Set_LuckyPoint(40)
			.Set_DamageRatio(-0.05)
			.Commit();
	}

	// 攻击-5%，攻速+30%
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::CRUD糕手>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::CRUD糕手)
			.Set_Name(U"CRUD糕手")
			.Set_Desc(U"CRUD糕手's desc")
			.Set_DamageRatio(-0.05)
			.Set_AttackSpeedRatio(0.3)
			.Commit();
	}

	// 幸运+20，移速-1，血量回复+15/s
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::动漫达人>(xx::Shared<xx::Node>& parent_) {
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
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::屎山代码>(xx::Shared<xx::Node>& parent_) {
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
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::福报>(xx::Shared<xx::Node>& parent_) {
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
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::归宅部>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::归宅部)
			.Set_Name(U"归宅部")
			.Set_Desc(U"归宅部's desc")
			.Set_MovementSpeedPoint(2)
			.Set_DodgeRatio(0.03)
			.Commit();
	}

	// 攻击+3%，幸运+5，血量回复+5/s，攻速+5%，最大血量+5%，暴击率+3%，爆伤+5%
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::为什么我不是沪爷>(xx::Shared<xx::Node>& parent_) {
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
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::彩票研究者>(xx::Shared<xx::Node>& parent_) {
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
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::社恐>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::社恐)
			.Set_Name(U"社恐")
			.Set_Desc(U"每3秒强制向后方冲刺0.5秒")
			.Set_DodgeRatio(0.15)
			.Set_AttackSpeedRatio(-0.1)
			.Commit();
	}

	// 杀死小怪时攻击+3%（一局内）
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::肉食主义>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::肉食主义)
			.Set_Name(U"肉食主义")
			.Set_Desc(U"杀死小怪时攻击+3%（一局内）")
			.Commit();
	}

	// 按下特殊键F进入10秒无敌状态，但是不能行动。一局一次
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::请假>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::请假)
			.Set_Name(U"请假")
			.Set_Desc(U"按下特殊键F进入10秒无敌状态，但是不能行动。一局一次")
			.Commit();
	}

	// 攻击+40%，攻速+20%，每回合最大HP-10%，收获-10%
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::技术债>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::技术债)
			.Set_Name(U"技术债")
			.Set_Desc(U"每回合最大HP-10%，收获-10%")
			.Set_DamageRatio(0.4)
			.Set_AttackSpeedRatio(0.2)
			.Commit();
	}

	// 攻击射出的子弹额外附加30%幸运值倍率的伤害
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::南无加特林机枪菩萨>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::南无加特林机枪菩萨)
			.Set_Name(U"南无加特林机枪菩萨")
			.Set_Desc(U"攻击射出的子弹额外附加30%幸运值倍率的伤害")
			.Commit();
	}

	// 最大血量提升，提升值为300%幸运值
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::南无阿弥陀佛>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::南无阿弥陀佛)
			.Set_Name(U"南无阿弥陀佛")
			.Set_Desc(U"最大血量提升，提升值为300%幸运值")
			.Commit();
	}

	// 攻击-3%，幸运-5，血量回复-5/s，攻速-5%，最大血量-5%，暴击率-3%，爆伤-5%，收获+200
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::沪爷>(xx::Shared<xx::Node>& parent_) {
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
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::坐在后面的好兄弟>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::坐在后面的好兄弟)
			.Set_Name(U"坐在后面的好兄弟")
			.Set_Desc(U"按F生成一个好兄弟，好兄弟会进入15秒无敌状态，和玩家拥有相同的攻击力，怪物会优先攻击好兄弟。15秒后结束。一局一次")
			.Commit();
	}

	// 生成3个好兄弟，拥有玩家50%攻击力和100%血量。自身攻击-25%，移速-1
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::团队Leader>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::团队Leader)
			.Set_Name(U"团队Leader")
			.Set_Desc(U"生成3个好兄弟，拥有玩家50%攻击力和100%血量")
			.Set_DamageRatio(-0.25)
			.Set_MovementSpeedPoint(-1)
			.Commit();
	}

	// 移速+5，护甲+30，攻击-100%
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::sudo_rm_rf>(xx::Shared<xx::Node>& parent_) {
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
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::裁员大动脉>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::裁员大动脉)
			.Set_Name(U"裁员大动脉")
			.Set_Desc(U"死亡时回复20%HP保证不死，一局一次。触发时对全体怪物造成100%幸运值+300%倍率的伤害。")
			.Commit();
	}

	// 最大血量+50%，伤害+50%，下一回合以1%血量起步
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::筋肉兄贵>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::筋肉兄贵)
			.Set_Name(U"筋肉兄贵")
			.Set_Desc(U"下一回合以1%血量起步")
			.Set_HealthRatio(0.5)
			.Set_DamageRatio(0.5)
			.Commit();
	}

	// 每回合结束时-50%~+100%代币
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::投资>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::投资)
			.Set_Name(U"投资")
			.Set_Desc(U"每回合结束时-50%~+100%代币")
			.Commit();
	}

	// 无作用
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::劳动法>(xx::Shared<xx::Node>& parent_) {
		return BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::劳动法)
			.Set_Name(U"劳动法")
			.Set_Desc(U"无作用")
			.Commit();
	}








	// todo: support mouse over show info
	struct SVContentBag : xx::Node {
		static constexpr XY cItemSize{ 128, 128 };
		static constexpr XY cItemMargin{ 10, 10 };
		float itemMarginX{ cItemMargin.x };

		xx::ScrollView* sv{};
		int32_t numCols{}, numRows{};
		xx::Listi32<BuffTypes> items;

		void Init(xx::ScrollView* sv_) {
			Node::Init(sv_->z + 1, {}, { 1,1 }, {}, sv_->size);
			sv = sv_;

			// sim
			for (int32_t i = 1; i < (int32_t)BuffTypes::__MaxValue__; ++i) {
				items.Emplace((BuffTypes)i);
			}

			UpdateSize();
		}

		void UpdateSize() {
			// calc bag grid size
			numCols = int32_t(sv->size.x + cItemMargin.x) / int32_t(cItemSize.x + cItemMargin.x);
			itemMarginX = (sv->size.x - cItemSize.x * numCols) / (numCols - 1);
			numRows = items.len / numCols;
			if (numRows * numCols < items.len) ++numRows;
			size = { numCols * cItemSize.x + (numCols - 1) * itemMarginX, numRows * cItemSize.y + (numRows - 1) * cItemMargin.y };
			sv->InitContentSize<false>(size); 
		}

		BuffTypes* TryGetMousePosItem() {
			if (sv->MousePosInArea()) {
#if 1
				auto rowIdxBegin = int32_t(size.y + parent->position.y - sv->size.y) / int32_t(cItemSize.y + cItemMargin.y);
				auto rowIdxEnd = rowIdxBegin + int32_t(sv->size.y) / int32_t(cItemSize.y + cItemMargin.y) + 1;
				auto basePos = worldMinXY;
				auto mp = xx::gEngine->mouse.pos;
				for (int32_t rowIdx = rowIdxBegin; rowIdx <= rowIdxEnd; ++rowIdx) {
					for (int32_t colIdx = 0; colIdx < numCols; ++colIdx) {
						auto itemIndex = rowIdx * numCols + colIdx;
						if (itemIndex >= items.len) return {};
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

		virtual void Draw() override {
			xx::Quad q;
			// calculate row cut range
			auto rowIdxBegin = int32_t(size.y + parent->position.y - sv->size.y) / int32_t(cItemSize.y + cItemMargin.y);
			auto rowIdxEnd = rowIdxBegin + int32_t(sv->size.y) / int32_t(cItemSize.y + cItemMargin.y) + 1;
			auto basePos = worldMinXY;
			for (int32_t rowIdx = rowIdxBegin; rowIdx <= rowIdxEnd; ++rowIdx) {
				for (int32_t colIdx = 0; colIdx < numCols; ++colIdx) {
					auto itemIndex = rowIdx * numCols + colIdx;
					if (itemIndex >= items.len) return;
					auto pos = basePos + XY{ colIdx * (cItemSize.x + itemMarginX) + cItemSize.x / 2, size.y - rowIdx * (cItemSize.y + cItemMargin.y) - cItemSize.y / 2 };
					//XY scale{ cItemSize.x / frame->textureRect.w, cItemSize.y / frame->textureRect.h };
					auto& frame = gLooper.res.buff_[(int32_t)items[itemIndex]];
					q.SetFrame(frame)/*.SetScale(scale)*/.SetPosition(pos).Draw();
				}
			}
		}
	};







	static constexpr XY cMargin{ 30, 30 };
	static constexpr XY cInfoMargin{ 10, 10 };

	inline void Test5::Init() {
		ui.Emplace()->Init();

		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy7m + XY{ 10, -10 }
			, Cfg::xy7a, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});


		sv = ui->MakeChildren<xx::ScrollView>();
		sv->Init(2, { -400, -300 }, { 1, 1 }, {}, { 800, 600 }, 1);
		sv->MakeChildren<xx::Scale9Sprite>()->Init(1, -cMargin, { 1,1 }, {}, sv->size + cMargin * 2, gLooper.btnCfg1);

		sv->MakeContent<SVContentBag>()->Init(sv);
		//auto&& rl = sv->MakeContent<xx::RichLabel>();
		//rl->Init(4, {}, { 1,1 }, {}, sv->size.x)
		//	.AddText(U" asdfasdfasd f sdf sadf sdf sd fs adf asdf sf sdf sadf sdf sd fs adf asdf sa fds df s df.\n");
		//rl->Commit();
		//sv->InitContentSize(rl->size);


		info.Emplace()->Init(1);
		info->MakeChildren<xx::Scale9Sprite>()->Init(1, -cInfoMargin, { 1,1 }, {}, 100, gLooper.btnCfg);


		gridSize = { 60, 60 };
		mapSize = 128 * gridSize;

		ground.Emplace()->Init(this, mapSize, gLooper.res.ground_cell2);
		player.Emplace<Player_1>()->Init(this);

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





		gLooper.DrawNode(ui);

		if (sv) {
			auto svc = (SVContentBag*)sv->children[0]->children[0].pointer;
			if (auto bt = svc->TryGetMousePosItem()) {
				if (info->children.len > 1) {
					info->children.PopBack();
				}
				info->position = gLooper.mouse.pos;
				info->FillTransRecursive();
				auto rl = MakeBuffRichLabels[(int32_t)*bt](info);
				auto bg = (xx::Scale9Sprite*)info->children[0].pointer;
				bg->size = { BuffRichLabelFiller::cFieldNameWidth + BuffRichLabelFiller::cValueWidth + cInfoMargin.x * 2, rl->size.y + cInfoMargin.y * 2 };
				
				gLooper.DrawNode(info);
			}
		}
	}
}
