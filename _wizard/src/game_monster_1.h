#pragma once

namespace Game {

	struct Monster_1_1;

	struct Monster_1 : Monster {
		static constexpr int32_t numChildren{ 9 };
		std::array<std::pair<XY, float>, numChildren> childrenInfos;	// pos + radians
		std::array<int32_t, numChildren> childrenIndexs;
		int32_t childrenIndexsCount{};
		int32_t nextSummonTime{}, summonDelay{};
		float radiansOffset{}, radiansOffsetInc{};
		void Init(Stage* stage_, XY bornPos, int32_t flyTargetsIdx_);
		int32_t Update() override;
		void TrySummonChildren();
		void FillChildrenInfo();
	};

	struct Monster_1_1 : Monster {
		xx::Weak<Monster_1> parent;
		float radiusInc{}, tarRadius{};
		int32_t nextShootTime{}, shootDelay{};
		void Init(xx::Weak<Monster_1> parent_, int32_t index_);
		int32_t Update() override;
		~Monster_1_1() override;
		void TryShoot();
	};

}
