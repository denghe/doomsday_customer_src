#pragma once

namespace Game {
	
	struct BuffNode : xx::Node {
		xx::RichLabel rl;
		void Draw() override;
	};

	template<BuffTypes bt> xx::Shared<BuffNode> MakeBuffNode(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::__Default__>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::_996>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::中药调理>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::跑步机>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::眼镜>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::肘击>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::加班餐>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::冲刺鞋子>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::旋转办公椅>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::塑料友情>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::钢板>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::海底捞>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::义愤填膺>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::国潮外卖>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::躺平>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::社交牛逼症>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::风水大师>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::CRUD糕手>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::动漫达人>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::屎山代码>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::福报>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::归宅部>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::为什么我不是沪爷>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::彩票研究者>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::社恐>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::肉食主义>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::请假>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::技术债>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::南无加特林机枪菩萨>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::南无阿弥陀佛>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::沪爷>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::坐在后面的好兄弟>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::团队Leader>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::sudo_rm_rf>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::裁员大动脉>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::筋肉兄贵>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::投资>(xx::Shared<xx::Node>& parent_);
	template<> xx::Shared<BuffNode> MakeBuffNode<BuffTypes::劳动法>(xx::Shared<xx::Node>& parent_);

	typedef xx::Shared<BuffNode>(*Func_MakeBuffNode)(xx::Shared<xx::Node>&);
	static constexpr Func_MakeBuffNode MakeBuffNodes[(int32_t)BuffTypes::__MaxValue__] = {
		MakeBuffNode<BuffTypes::__Default__>,
		MakeBuffNode<BuffTypes::_996>,
		MakeBuffNode<BuffTypes::中药调理>,
		MakeBuffNode<BuffTypes::跑步机>,
		MakeBuffNode<BuffTypes::眼镜>,
		MakeBuffNode<BuffTypes::肘击>,
		MakeBuffNode<BuffTypes::加班餐>,
		MakeBuffNode<BuffTypes::冲刺鞋子>,
		MakeBuffNode<BuffTypes::旋转办公椅>,
		MakeBuffNode<BuffTypes::塑料友情>,
		MakeBuffNode<BuffTypes::钢板>,
		MakeBuffNode<BuffTypes::海底捞>,
		MakeBuffNode<BuffTypes::义愤填膺>,
		MakeBuffNode<BuffTypes::国潮外卖>,
		MakeBuffNode<BuffTypes::躺平>,
		MakeBuffNode<BuffTypes::社交牛逼症>,
		MakeBuffNode<BuffTypes::风水大师>,
		MakeBuffNode<BuffTypes::CRUD糕手>,
		MakeBuffNode<BuffTypes::动漫达人>,
		MakeBuffNode<BuffTypes::屎山代码>,
		MakeBuffNode<BuffTypes::福报>,
		MakeBuffNode<BuffTypes::归宅部>,
		MakeBuffNode<BuffTypes::为什么我不是沪爷>,
		MakeBuffNode<BuffTypes::彩票研究者>,
		MakeBuffNode<BuffTypes::社恐>,
		MakeBuffNode<BuffTypes::肉食主义>,
		MakeBuffNode<BuffTypes::请假>,
		MakeBuffNode<BuffTypes::技术债>,
		MakeBuffNode<BuffTypes::南无加特林机枪菩萨>,
		MakeBuffNode<BuffTypes::南无阿弥陀佛>,
		MakeBuffNode<BuffTypes::沪爷>,
		MakeBuffNode<BuffTypes::坐在后面的好兄弟>,
		MakeBuffNode<BuffTypes::团队Leader>,
		MakeBuffNode<BuffTypes::sudo_rm_rf>,
		MakeBuffNode<BuffTypes::裁员大动脉>,
		MakeBuffNode<BuffTypes::筋肉兄贵>,
		MakeBuffNode<BuffTypes::投资>,
		MakeBuffNode<BuffTypes::劳动法>,
	};


	struct Test5 : Stage {
		xx::Shared<xx::Node> ui;
		xx::Weak<xx::ScrollView> sv;

		void Init() override;
		void Update() override;
		void Draw() override;
	};

}
