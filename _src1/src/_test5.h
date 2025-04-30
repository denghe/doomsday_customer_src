#pragma once

namespace Game {
	
	template<BuffTypes bt> xx::RichLabel* MakeBuffRichLabel(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::__Default__>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::_996>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::中药调理>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::跑步机>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::眼镜>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::肘击>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::加班餐>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::冲刺鞋子>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::旋转办公椅>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::塑料友情>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::钢板>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::海底捞>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::义愤填膺>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::国潮外卖>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::躺平>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::社交牛逼症>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::风水大师>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::CRUD糕手>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::动漫达人>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::屎山代码>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::福报>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::归宅部>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::为什么我不是沪爷>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::彩票研究者>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::社恐>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::肉食主义>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::请假>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::技术债>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::南无加特林机枪菩萨>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::南无阿弥陀佛>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::沪爷>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::坐在后面的好兄弟>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::团队Leader>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::sudo_rm_rf>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::裁员大动脉>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::筋肉兄贵>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::投资>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::劳动法>(xx::Node* parent_, float nameWidth_, float valueWidth_, XY pos_, XY anchor_);

	typedef xx::RichLabel*(*Func_MakeBuffRichLabel)(xx::Node*, float, float, XY, XY);
	static constexpr Func_MakeBuffRichLabel MakeBuffRichLabels[(int32_t)BuffTypes::__MaxValue__] = {
		MakeBuffRichLabel<BuffTypes::__Default__>,
		MakeBuffRichLabel<BuffTypes::_996>,
		MakeBuffRichLabel<BuffTypes::中药调理>,
		MakeBuffRichLabel<BuffTypes::跑步机>,
		MakeBuffRichLabel<BuffTypes::眼镜>,
		MakeBuffRichLabel<BuffTypes::肘击>,
		MakeBuffRichLabel<BuffTypes::加班餐>,
		MakeBuffRichLabel<BuffTypes::冲刺鞋子>,
		MakeBuffRichLabel<BuffTypes::旋转办公椅>,
		MakeBuffRichLabel<BuffTypes::塑料友情>,
		MakeBuffRichLabel<BuffTypes::钢板>,
		MakeBuffRichLabel<BuffTypes::海底捞>,
		MakeBuffRichLabel<BuffTypes::义愤填膺>,
		MakeBuffRichLabel<BuffTypes::国潮外卖>,
		MakeBuffRichLabel<BuffTypes::躺平>,
		MakeBuffRichLabel<BuffTypes::社交牛逼症>,
		MakeBuffRichLabel<BuffTypes::风水大师>,
		MakeBuffRichLabel<BuffTypes::CRUD糕手>,
		MakeBuffRichLabel<BuffTypes::动漫达人>,
		MakeBuffRichLabel<BuffTypes::屎山代码>,
		MakeBuffRichLabel<BuffTypes::福报>,
		MakeBuffRichLabel<BuffTypes::归宅部>,
		MakeBuffRichLabel<BuffTypes::为什么我不是沪爷>,
		MakeBuffRichLabel<BuffTypes::彩票研究者>,
		MakeBuffRichLabel<BuffTypes::社恐>,
		MakeBuffRichLabel<BuffTypes::肉食主义>,
		MakeBuffRichLabel<BuffTypes::请假>,
		MakeBuffRichLabel<BuffTypes::技术债>,
		MakeBuffRichLabel<BuffTypes::南无加特林机枪菩萨>,
		MakeBuffRichLabel<BuffTypes::南无阿弥陀佛>,
		MakeBuffRichLabel<BuffTypes::沪爷>,
		MakeBuffRichLabel<BuffTypes::坐在后面的好兄弟>,
		MakeBuffRichLabel<BuffTypes::团队Leader>,
		MakeBuffRichLabel<BuffTypes::sudo_rm_rf>,
		MakeBuffRichLabel<BuffTypes::裁员大动脉>,
		MakeBuffRichLabel<BuffTypes::筋肉兄贵>,
		MakeBuffRichLabel<BuffTypes::投资>,
		MakeBuffRichLabel<BuffTypes::劳动法>,
	};

	struct UI_BuffShop;

	struct UI_BuffsContent;
	struct UI_BuffBag : xx::ScrollView {
		static constexpr XY cMargin{ 30, 30 };

		UI_BuffsContent* content{};	// shortcut for easy access
		void Init(Stage* stage_, int z_, XY const& position_, XY const& scale_, XY const& anchor_, XY const& size_);
	};

	// for UI_BuffBag only
	struct UI_BuffsContent : xx::Node {
		static constexpr XY cScale{ 0.5 };
		static constexpr XY cItemSize{ 64, 64 };
		static constexpr XY cItemMargin{ 10, 10 };

		Stage* stage{};
		UI_BuffBag* buffBag{};
		float itemMarginX{ cItemMargin.x };
		int32_t numCols{}, numRows{};

		UI_BuffsContent* Init(Stage* stage_, UI_BuffBag* buffBag_);
		void Refresh();
		BuffTypes* TryGetMousePosItem();	// for show info
		void Draw() override;
	};


	struct UI_BuffInfo : xx::Node {
		static constexpr float cFieldNameWidth{ 200 };
		static constexpr float cValueWidth{ 70 };
		static constexpr XY cInfoMargin{ 20, 20 };
		void Init();
		void Set(BuffTypes bt, XY pos);
	};

	// UI_BuffShopGoodsList's children
	struct UI_BuffShopGoodsItem : xx::EmptyButton {
		Stage* stage{};
		void Init(Stage* stage_, BuffTypes bt, int z_, XY const& pos_, XY const& anchor_, XY const& size_);
	};

	// UI_BuffShop's children
	struct UI_BuffShopGoodsList : xx::Node {
		Stage* stage{};
		void Init(Stage* stage_, int32_t z_, XY pos_, XY size_);
		void Refresh();
	};

	struct UI_PlayerStat : xx::Node {
		Stage* stage{};
		void Init(Stage* stage_, int32_t z_, XY pos_);
		void Refresh();
	};

	struct UI_BuffShop : xx::Node {
		Stage* stage{};

		// shortcuts for easy access
		UI_BuffShopGoodsList* goodsList{};
		UI_PlayerStat* playerStat{};
		UI_BuffBag* buffBag{};

		void Init(Stage* stage_);
	};



	struct Test5 : Stage {
		xx::Shared<xx::Node> ui;
		xx::Shared<UI_BuffShop> buffShop;
		xx::Shared<UI_BuffInfo> buffInfo;

		void Init() override;
		void Update() override;
		void Draw() override;
	};

}
