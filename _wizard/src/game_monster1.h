#pragma once

namespace Game {

	struct Monster1_1;

	struct Monster1 : Monster {
		static constexpr int32_t numChildren{ 9 };
		struct ChildrenInfo {
			XY offset{};
			float radians{}, cos{}, sin{};
		};

		std::array<ChildrenInfo, numChildren> childrenInfos;	// pos + radians
		std::array<int32_t, numChildren> childrenIndexs;
		int32_t childrenIndexsCount{};
		int32_t nextSummonTime{}, summonDelay{};
		float radiansOffset{}, radiansOffsetInc{};
		void Init(Stage* stage_, XY bornPos, int32_t flyTargetsIdx_);
		int32_t Update() override;
		void TrySummonChildren();
		void FillChildrenInfo();
	};

	struct Monster1_1 : Monster {
		xx::Weak<Monster1> parent;
		float radiusInc{}, tarRadius{};
		int32_t nextShootTime{}, shootDelay{};
		void Init(xx::Weak<Monster1> parent_, int32_t index_);
		int32_t Update() override;
		~Monster1_1() override;
		void TryShoot();
	};


}
