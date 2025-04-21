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

	struct BuffContainer;

	template<BuffTypes bt> void BuffAddTo(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::__Default__>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::_996>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::中药调理>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::跑步机>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::眼镜>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::肘击>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::加班餐>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::冲刺鞋子>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::旋转办公椅>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::塑料友情>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::钢板>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::海底捞>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::义愤填膺>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::国潮外卖>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::躺平>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::社交牛逼症>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::风水大师>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::CRUD糕手>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::动漫达人>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::屎山代码>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::福报>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::归宅部>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::为什么我不是沪爷>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::彩票研究者>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::社恐>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::肉食主义>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::请假>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::技术债>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::南无加特林机枪菩萨>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::南无阿弥陀佛>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::沪爷>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::坐在后面的好兄弟>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::团队Leader>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::sudo_rm_rf>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::裁员大动脉>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::筋肉兄贵>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::投资>(BuffContainer& bc);
	template<> void BuffAddTo<BuffTypes::劳动法>(BuffContainer& bc);

	typedef void (*Func_BuffAddTo)(BuffContainer&);
	static constexpr Func_BuffAddTo BuffAddTos[(int32_t)BuffTypes::__MaxValue__] = {
		BuffAddTo<BuffTypes::__Default__>,
		BuffAddTo<BuffTypes::_996>,
		BuffAddTo<BuffTypes::中药调理>,
		BuffAddTo<BuffTypes::跑步机>,
		BuffAddTo<BuffTypes::眼镜>,
		BuffAddTo<BuffTypes::肘击>,
		BuffAddTo<BuffTypes::加班餐>,
		BuffAddTo<BuffTypes::冲刺鞋子>,
		BuffAddTo<BuffTypes::旋转办公椅>,
		BuffAddTo<BuffTypes::塑料友情>,
		BuffAddTo<BuffTypes::钢板>,
		BuffAddTo<BuffTypes::海底捞>,
		BuffAddTo<BuffTypes::义愤填膺>,
		BuffAddTo<BuffTypes::国潮外卖>,
		BuffAddTo<BuffTypes::躺平>,
		BuffAddTo<BuffTypes::社交牛逼症>,
		BuffAddTo<BuffTypes::风水大师>,
		BuffAddTo<BuffTypes::CRUD糕手>,
		BuffAddTo<BuffTypes::动漫达人>,
		BuffAddTo<BuffTypes::屎山代码>,
		BuffAddTo<BuffTypes::福报>,
		BuffAddTo<BuffTypes::归宅部>,
		BuffAddTo<BuffTypes::为什么我不是沪爷>,
		BuffAddTo<BuffTypes::彩票研究者>,
		BuffAddTo<BuffTypes::社恐>,
		BuffAddTo<BuffTypes::肉食主义>,
		BuffAddTo<BuffTypes::请假>,
		BuffAddTo<BuffTypes::技术债>,
		BuffAddTo<BuffTypes::南无加特林机枪菩萨>,
		BuffAddTo<BuffTypes::南无阿弥陀佛>,
		BuffAddTo<BuffTypes::沪爷>,
		BuffAddTo<BuffTypes::坐在后面的好兄弟>,
		BuffAddTo<BuffTypes::团队Leader>,
		BuffAddTo<BuffTypes::sudo_rm_rf>,
		BuffAddTo<BuffTypes::裁员大动脉>,
		BuffAddTo<BuffTypes::筋肉兄贵>,
		BuffAddTo<BuffTypes::投资>,
		BuffAddTo<BuffTypes::劳动法>,
	};

	struct Creature;
	struct BuffContainer {
		static constexpr int32_t numBuffers{ (int32_t)BuffTypes::__MaxValue__ };

		xx::Weak<Creature> owner;
		std::array<int32_t, numBuffers> nums;
		StatPanel sp;								// sum
		bool _跑步机_used{};
		bool _筋肉兄贵_used{};
		Stat_t _肉食主义_damageScaleCount{};
		int32_t _裁员大动脉_usedCount{};
		// ...

		void Init(Creature* owner_);

		int32_t& At(BuffTypes bt) const;

		// for assert
		bool IsLimited(BuffTypes bt) const;

		// for shopping. return value for assert
		bool TryAdd(BuffTypes bt);

		// todo: get drop list with exclude limited ?
	};

}
