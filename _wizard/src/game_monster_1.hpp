#pragma once

namespace Game {

	inline void Monster_1::Init(Stage* stage_, XY bornPos, int32_t flyTargetsIdx_) {
		assert(!stage);
		stage = stage_;
		frame = gLooper.res.monster_1;
		lightFrame = gLooper.res.light_monster_1;
		pos = bornPos;
		radians = float(M_PI) * 0.5f;
		radius = 25.f;
		flyTargetsIdx = flyTargetsIdx_;
		mp.Init();

		radiansOffset = {};
		radiansOffsetInc = float(M_PI * 2.f) / (4.f * Cfg::fps);
		FillChildrenInfo();
		childrenIndexsCount = numChildren;
		for (int32_t i = 0; i < numChildren; ++i) {
			childrenIndexs[i] = i;
		}
		summonDelay = int32_t(0.5f * Cfg::fps);
		shaker.Shake(20, 500.f * Cfg::frameDelay, 999999999, stage->time);
	}

	XX_INLINE void Monster_1::FillChildrenInfo() {
		static constexpr float distance{ 25.f + 30.f };
		auto stepRadians = float(M_PI * 2 / numChildren);
		for (int32_t i = 0; i < numChildren; ++i) {
			auto r = radiansOffset + stepRadians * i;
			childrenInfos[i].second = r;
			childrenInfos[i].first = { std::cosf(r) * distance, std::sinf(r) * distance };
		}
	}

	inline int32_t Monster_1::Update() {
		if (stunEndTime >= stage->time) return 0;	// when stun: pause all action
		XX_BEGIN(_n);
		while (!MoveTo(GetFlyTargetPos<false>(), cFlySpeed)) {
			XX_YIELD_I(_n);
		}
		while (true) {
			++stage->numReadyMonsters;
			shaker.Update(stage->rnd, stage->time);
			MoveTo(GetFlyTargetPos<true>() + shaker.offset, cMoveSpeed);
			HandleBlocks();
			SyncPos();
			radiansOffset += radiansOffsetInc;// todo: limit radiansOffset to pi
			FillChildrenInfo();
			TrySummonChildren();
			XX_YIELD_I(_n);
		}
		XX_END(_n);
		return 0;
	}

	XX_INLINE void Monster_1::TrySummonChildren() {
		if (!childrenIndexsCount) return;
		if (nextSummonTime >= stage->time) return;
		nextSummonTime = stage->time + summonDelay;
		auto m = xx::MakeShared<Monster_1_1>();
		m->Init(xx::WeakFromThis(this), childrenIndexs[--childrenIndexsCount]);
		stage->monsters.Add(std::move(m));
	}

	/*******************************************************************************/
	/*******************************************************************************/

	inline void Monster_1_1::Init(xx::Weak<Monster_1> parent_, int32_t index_) {
		assert(!stage);
		parent = std::move(parent_);
		flyTargetsIdx = index_;
		stage = parent->stage;
		frame = gLooper.res.monster_1;
		lightFrame = gLooper.res.light_monster_1;
		mp.Init();
		radius = 5.f;
		tarRadius = 20.f;
		radiusInc = (tarRadius - radius) / (0.7f * Cfg::fps);
		shootDelay = int32_t(1.f * Cfg::fps);
		xx::CoutN(flyTargetsIdx);
	}

	inline int32_t Monster_1_1::Update() {
		if (!parent) {
			// todo: death effect
			return 1;
		}
		auto& pr = parent->childrenInfos[flyTargetsIdx];
		pos = parent->pos + pr.first;
		radians = pr.second;
		if (radius < tarRadius) {
			radius += radiusInc;
			HandleBlocks();
			SyncPos();
		}
		else {
			HandleBlocks();
			SyncPos();
			TryShoot();
		}
		return 0;
	}
	inline Monster_1_1::~Monster_1_1() {
		if (parent) {
			parent->childrenIndexs[parent->childrenIndexsCount++] = flyTargetsIdx;
		}
	}

	XX_INLINE void Monster_1_1::TryShoot() {
		if (nextShootTime >= stage->time) return;
		nextShootTime = stage->time + shootDelay;
		// todo: make bullet
	}
}
