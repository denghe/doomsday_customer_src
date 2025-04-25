#pragma once

namespace Game {

	struct BuffRichLabelFiller {
		static constexpr float cFieldNameWidth{ 300 };
		// todo: more cfg

		xx::RichLabel* rl{};
		BuffTypes bt{};
		std::u32string name, desc, vStr;

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

		XX_INLINE std::u32string_view ToStr(Stat_t v) {
			vStr.resize(xx::ToStringEN<char32_t>(v, vStr.data()));
			return vStr;
		}

		BuffRichLabelFiller(xx::Shared<xx::Node>& parent_) {
			auto&& rl_ = parent_->MakeChildren<xx::RichLabel>();
			rl_->Init(4, {}, { 1,1 }, {}, 300);
			rl = rl_.pointer;
			vStr.resize(12);
		}

		XX_INLINE BuffRichLabelFiller& Set_BuffType(BuffTypes bt_) { bt = bt_; return *this; }
		XX_INLINE BuffRichLabelFiller& Set_Name(std::u32string name_) { name = std::move(name_); return *this; }
		XX_INLINE BuffRichLabelFiller& Set_Desc(std::u32string desc_) { desc_ = std::move(desc_); return *this; }

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

		XX_INLINE void TryAppend(std::u32string_view fieldName, std::optional<Stat_t>& v) {
			if (v.has_value()) {
				// todo: posstive set color blue? negative set color red?
				rl->AddLimitedWidthText(fieldName, cFieldNameWidth).AddRightText(ToStr(*v));
			}
		}

		inline void Commit() {
			// todo: fill icon
			// todo: fill name
			TryAppend(U"基础血量", healthPoint);
			TryAppend(U"血量系数", healthRatio);
			TryAppend(U"最大血量", healthPointMax);
			TryAppend(U"血量回复", healthRegeneration);
			TryAppend(U"防御值", defensePoint);
			TryAppend(U"防御系数", defenseRatio);
			TryAppend(U"闪避值", dodgePoint);
			TryAppend(U"闪避系数", dodgeRatio);
			TryAppend(U"闪避率", dodgeChance);
			TryAppend(U"移动值", movementSpeedPoint);
			TryAppend(U"移动速度", movementSpeed);
			TryAppend(U"帧移动速度", movementSpeedPerFrame);
			TryAppend(U"伤害倍率", damageRatio);
			TryAppend(U"暴击概率", criticalChance);
			TryAppend(U"暴击倍率", criticalBonusRatio);
			TryAppend(U"攻速", attackSpeed);
			TryAppend(U"攻速系数", attackSpeedRatio);
			TryAppend(U"幸运值", luckyPoint);
			TryAppend(U"收获值", harvestRatio);
			TryAppend(U"视野", fieldOfVision);
			// todo: fill desc. analysis color control char?
			rl->Commit();
		}
	};


	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::__Default__>(xx::Shared<xx::Node>& parent_) {
		assert(false);
		return {};
	}

	// 攻击+20%，攻速-5%，最大血量-5%
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::_996>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::_996)
			.Set_Name(U"996")
			.Set_Desc(U"996's desc")
			.Set_DamageRatio(0.2f)
			.Set_AttackSpeedRatio(-0.05f)
			.Set_HealthRatio(-0.05f)
			.Commit();
		return {};
	}

	// 血量回复+10/s
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::中药调理>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::中药调理)
			.Set_Name(U"中药调理")
			.Set_Desc(U"中药调理's desc")
			.Set_HealthRegeneration(10)
			.Commit();
		return {};
	}

	// 最大血量+15%，下一回合以30%血量起步
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::跑步机>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::跑步机)
			.Set_Name(U"跑步机")
			.Set_Desc(U"下一回合以30%血量起步")
			.Set_HealthRatio(0.15)
			.Commit();
		return {};
	}

	// 范围+10，幸运-5
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::眼镜>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::眼镜)
			.Set_Name(U"眼镜")
			.Set_Desc(U"眼镜's desc")
			.Set_FieldOfVision(10)
			.Set_LuckyPoint(-5)
			.Commit();
		return {};
	}

	// 射速+5%
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::肘击>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::肘击)
			.Set_Name(U"肘击")
			.Set_Desc(U"肘击's desc")
			.Set_AttackSpeedRatio(0.05)
			.Commit();
		return {};
	}

	// 攻击+15%，血量回复-5/s
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::加班餐>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::加班餐)
			.Set_Name(U"加班餐")
			.Set_Desc(U"加班餐's desc")
			.Set_DamageRatio(0.15)
			.Set_HealthRegeneration(-5)
			.Commit();
		return {};
	}

	// 移速+1
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::冲刺鞋子>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::冲刺鞋子)
			.Set_Name(U"冲刺鞋子")
			.Set_Desc(U"冲刺鞋子's desc")
			.Set_MovementSpeedPoint(1)
			.Commit();
		return {};
	}

	// 攻速+20%，移速-1
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::旋转办公椅>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::旋转办公椅)
			.Set_Name(U"旋转办公椅")
			.Set_Desc(U"旋转办公椅's desc")
			.Set_AttackSpeedRatio(0.2)
			.Set_MovementSpeedPoint(-1)
			.Commit();
		return {};
	}

	// 攻速+10%，幸运-5，收获+5%
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::塑料友情>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::塑料友情)
			.Set_Name(U"塑料友情")
			.Set_Desc(U"塑料友情's desc")
			.Set_AttackSpeedRatio(0.1)
			.Set_LuckyPoint(-5)
			.Set_HarvestRatio(0.05)
			.Commit();
		return {};
	}

	// 护甲+5
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::钢板>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::钢板)
			.Set_Name(U"钢板")
			.Set_Desc(U"钢板's desc")
			.Set_DefensePoint(5)
			.Commit();
		return {};
	}

	// 最大血量+5%，血量回复+10，伤害-5%
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::海底捞>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::海底捞)
			.Set_Name(U"海底捞")
			.Set_Desc(U"海底捞's desc")
			.Set_HealthRatio(0.05)
			.Set_HealthRegeneration(10)
			.Set_DamageRatio(-0.05)
			.Commit();
		return {};
	}

	// 暴击伤害+15%
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::义愤填膺>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::义愤填膺)
			.Set_Name(U"义愤填膺")
			.Set_Desc(U"义愤填膺's desc")
			.Set_CriticalBonusRatio(0.15)
			.Commit();
		return {};
	}

	// 杀死小怪时有70%概率+30HP，30%概率-30HP
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::国潮外卖>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::国潮外卖)
			.Set_Name(U"国潮外卖")
			.Set_Desc(U"杀死小怪时有70%概率+30HP，30%概率-30HP")
			.Commit();
		return {};
	}

	// 攻击-20%，幸运+30，血量回复+15/s
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::躺平>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::躺平)
			.Set_Name(U"躺平")
			.Set_Desc(U"躺平's desc")
			.Set_DamageRatio(-0.2)
			.Set_LuckyPoint(30)
			.Set_HealthRegeneration(15)
			.Commit();
		return {};
	}

	// 攻击+30%，攻速+10%，每5秒不受控制的固定向前方冲刺0.5秒
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::社交牛逼症>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::社交牛逼症)
			.Set_Name(U"社交牛逼症")
			.Set_Desc(U"每5秒不受控制的固定向前方冲刺0.5秒")
			.Set_DamageRatio(0.3)
			.Set_AttackSpeedRatio(0.1)
			.Commit();
		return {};
	}

	// 幸运+40，攻击-5%
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::风水大师>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::风水大师)
			.Set_Name(U"风水大师")
			.Set_Desc(U"风水大师's desc")
			.Set_LuckyPoint(40)
			.Set_DamageRatio(-0.05)
			.Commit();
		return {};
	}

	// 攻击-5%，攻速+30%
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::CRUD糕手>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::CRUD糕手)
			.Set_Name(U"CRUD糕手")
			.Set_Desc(U"CRUD糕手's desc")
			.Set_DamageRatio(-0.05)
			.Set_AttackSpeedRatio(0.3)
			.Commit();
		return {};
	}

	// 幸运+20，移速-1，血量回复+15/s
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::动漫达人>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::动漫达人)
			.Set_Name(U"动漫达人")
			.Set_Desc(U"动漫达人's desc")
			.Set_LuckyPoint(20)
			.Set_MovementSpeedPoint(-1)
			.Set_HealthRegeneration(15)
			.Commit();
		return {};
	}

	// 攻击+15%，攻速+30%，血量回复-20/s
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::屎山代码>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::屎山代码)
			.Set_Name(U"屎山代码")
			.Set_Desc(U"屎山代码's desc")
			.Set_DamageRatio(0.15)
			.Set_AttackSpeedRatio(0.3)
			.Set_HealthRegeneration(-20)
			.Commit();
		return {};
	}

	// 最大血量-50%，攻速+100%，攻击-20%
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::福报>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::福报)
			.Set_Name(U"福报")
			.Set_Desc(U"福报's desc")
			.Set_HealthRatio(-0.5)
			.Set_AttackSpeedRatio(1)
			.Set_DamageRatio(-0.2)
			.Commit();
		return {};
	}

	// 移速+2，闪避+3%
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::归宅部>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::归宅部)
			.Set_Name(U"归宅部")
			.Set_Desc(U"归宅部's desc")
			.Set_MovementSpeedPoint(2)
			.Set_DodgeRatio(0.03)
			.Commit();
		return {};
	}

	// 攻击+3%，幸运+5，血量回复+5/s，攻速+5%，最大血量+5%，暴击率+3%，爆伤+5%
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::为什么我不是沪爷>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
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
		return {};
	}

	// 幸运 +60，攻击 -10%，血量回复 -5/s
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::彩票研究者>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::彩票研究者)
			.Set_Name(U"彩票研究者")
			.Set_Desc(U"彩票研究者's desc")
			.Set_LuckyPoint(60)
			.Set_DamageRatio(-0.1)
			.Set_HealthRegeneration(-5)
			.Commit();
		return {};
	}

	// 闪避+15%，攻速 -10%，每3秒强制向后方冲刺0.5秒
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::社恐>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::社恐)
			.Set_Name(U"社恐")
			.Set_Desc(U"每3秒强制向后方冲刺0.5秒")
			.Set_DodgeRatio(0.15)
			.Set_AttackSpeedRatio(-0.1)
			.Commit();
		return {};
	}

	// 杀死小怪时攻击+3%（一局内）
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::肉食主义>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::肉食主义)
			.Set_Name(U"肉食主义")
			.Set_Desc(U"杀死小怪时攻击+3%（一局内）")
			.Commit();
		return {};
	}

	// 按下特殊键F进入10秒无敌状态，但是不能行动。一局一次
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::请假>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::请假)
			.Set_Name(U"请假")
			.Set_Desc(U"按下特殊键F进入10秒无敌状态，但是不能行动。一局一次")
			.Commit();
		return {};
	}

	// 攻击+40%，攻速+20%，每回合最大HP-10%，收获-10%
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::技术债>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::技术债)
			.Set_Name(U"技术债")
			.Set_Desc(U"每回合最大HP-10%，收获-10%")
			.Set_DamageRatio(0.4)
			.Set_AttackSpeedRatio(0.2)
			.Commit();
		return {};
	}

	// 攻击射出的子弹额外附加30%幸运值倍率的伤害
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::南无加特林机枪菩萨>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::南无加特林机枪菩萨)
			.Set_Name(U"南无加特林机枪菩萨")
			.Set_Desc(U"攻击射出的子弹额外附加30%幸运值倍率的伤害")
			.Commit();
		return {};
	}

	// 最大血量提升，提升值为300%幸运值
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::南无阿弥陀佛>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::南无阿弥陀佛)
			.Set_Name(U"南无阿弥陀佛")
			.Set_Desc(U"最大血量提升，提升值为300%幸运值")
			.Commit();
		return {};
	}

	// 攻击-3%，幸运-5，血量回复-5/s，攻速-5%，最大血量-5%，暴击率-3%，爆伤-5%，收获+200
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::沪爷>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
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
		return {};
	}

	// 按F生成一个好兄弟，好兄弟会进入15秒无敌状态，和玩家拥有相同的攻击力，怪物会优先攻击好兄弟。15秒后结束。一局一次
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::坐在后面的好兄弟>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::坐在后面的好兄弟)
			.Set_Name(U"坐在后面的好兄弟")
			.Set_Desc(U"按F生成一个好兄弟，好兄弟会进入15秒无敌状态，和玩家拥有相同的攻击力，怪物会优先攻击好兄弟。15秒后结束。一局一次")
			.Commit();
		return {};
	}

	// 生成3个好兄弟，拥有玩家50%攻击力和100%血量。自身攻击-25%，移速-1
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::团队Leader>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::团队Leader)
			.Set_Name(U"团队Leader")
			.Set_Desc(U"生成3个好兄弟，拥有玩家50%攻击力和100%血量")
			.Set_DamageRatio(-0.25)
			.Set_MovementSpeedPoint(-1)
			.Commit();
		return {};
	}

	// 移速+5，护甲+30，攻击-100%
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::sudo_rm_rf>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::sudo_rm_rf)
			.Set_Name(U"sudo_rm_rf")
			.Set_Desc(U"sudo_rm_rf's desc")
			.Set_MovementSpeedPoint(5)
			.Set_DefensePoint(30)
			.Set_DamageRatio(-1)
			.Commit();
		return {};
	}

	// 死亡时回复20%HP保证不死，一局一次。触发时对全体怪物造成100%幸运值+300%倍率的伤害。
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::裁员大动脉>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::裁员大动脉)
			.Set_Name(U"裁员大动脉")
			.Set_Desc(U"死亡时回复20%HP保证不死，一局一次。触发时对全体怪物造成100%幸运值+300%倍率的伤害。")
			.Commit();
		return {};
	}

	// 最大血量+50%，伤害+50%，下一回合以1%血量起步
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::筋肉兄贵>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::筋肉兄贵)
			.Set_Name(U"筋肉兄贵")
			.Set_Desc(U"下一回合以1%血量起步")
			.Set_HealthRatio(0.5)
			.Set_DamageRatio(0.5)
			.Commit();
		return {};
	}

	// 每回合结束时-50%~+100%代币
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::投资>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::投资)
			.Set_Name(U"投资")
			.Set_Desc(U"每回合结束时-50%~+100%代币")
			.Commit();
		return {};
	}

	// 无作用
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::劳动法>(xx::Shared<xx::Node>& parent_) {
		BuffRichLabelFiller{ parent_ }
			.Set_BuffType(BuffTypes::劳动法)
			.Set_Name(U"劳动法")
			.Set_Desc(U"无作用")
			.Commit();
		return {};
	}

	void BuffNode::Draw() {

	}


	inline void Test5::Init() {
		ui.Emplace()->Init();

		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy7m + XY{ 10, -10 }
			, Cfg::xy7a, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});


		sv = ui->MakeChildren<xx::ScrollView>();
		sv->Init(2, { 50, 50 }, { 1, 1 }, {}, { 200, 200 }, { 50, 50 });
		sv->MakeChildren<xx::Scale9Sprite>()->Init(1, {}, { 1,1 }, {}, sv->size, gLooper.btnCfg1);
		//sv->MakeChildren<SVContent>()->Init(3, {}, { 1,1 }, {}, sv->size);

		auto&& rl = sv->MakeContent<xx::RichLabel>();
		rl->Init(4, {}, { 1,1 }, {}, sv->size.x)
			.AddText(U" asdfasdfasd f sdf sadf sdf sd fs adf asdf sf sdf sadf sdf sd fs adf asdf sa fds df s df.\n");
		rl->Commit();

		sv->InitContentSize(rl->size);


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
	}
}
