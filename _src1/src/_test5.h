#pragma once

namespace Game {
	
	template<BuffTypes bt> xx::RichLabel* MakeBuffRichLabel(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::__Default__>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::_996>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::中药调理>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::跑步机>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::眼镜>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::肘击>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::加班餐>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::冲刺鞋子>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::旋转办公椅>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::塑料友情>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::钢板>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::海底捞>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::义愤填膺>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::国潮外卖>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::躺平>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::社交牛逼症>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::风水大师>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::CRUD糕手>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::动漫达人>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::屎山代码>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::福报>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::归宅部>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::为什么我不是沪爷>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::彩票研究者>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::社恐>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::肉食主义>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::请假>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::技术债>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::南无加特林机枪菩萨>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::南无阿弥陀佛>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::沪爷>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::坐在后面的好兄弟>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::团队Leader>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::sudo_rm_rf>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::裁员大动脉>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::筋肉兄贵>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::投资>(xx::Shared<xx::Node>& parent_);
	template<> xx::RichLabel* MakeBuffRichLabel<BuffTypes::劳动法>(xx::Shared<xx::Node>& parent_);

	typedef xx::RichLabel*(*Func_MakeBuffRichLabel)(xx::Shared<xx::Node>&);
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


	struct Test5 : Stage {
		xx::Shared<xx::Node> ui;
		xx::Shared<xx::Node> info;
		xx::Weak<xx::ScrollView> sv;

		void Init() override;
		void Update() override;
		void Draw() override;
	};

}
