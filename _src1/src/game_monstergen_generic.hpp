#pragma once

namespace Game {
	template<typename T>
	int32_t MonsterGen_Generic<T>::Update() {
		//CreateFromRect<T>();
		//CreateFromDoughnut<T>();
		CreateFromFullMap<T>();
		return 0;
	}

	/****************************************************************************************************************************************/
	/****************************************************************************************************************************************/

	template<typename T>
	inline void MonsterGen_Group<T>::Init(Stage* stage_, float activeSeconds_, float destroySeconds_, int32_t numMonsters_, float radius_, float durationSeconds_, int32_t numGroups_) {
		stage = stage_;
		activeTime = stage_->time + int32_t(Cfg::fps * activeSeconds_);
		destroyTime = stage_->time + int32_t(Cfg::fps * destroySeconds_);

		radius = radius_;
		numMonsters = numMonsters_;
		numGroups = numGroups_;
		delayNumFrames = int32_t(Cfg::fps * durationSeconds_) / numMonsters_;

		auto d = destroyTime - activeTime;
		assert(d >= delayNumFrames * numGroups_);
		groupDelayNumFrames = d / numGroups_ - delayNumFrames;
	}

	template<typename T>
	inline int32_t MonsterGen_Group<T>::Update() {
		XX_BEGIN(n);

		for (j = 0; j < numGroups; ++j) {

			pos.x = stage->rnd.Next<float>(radius, stage->mapSize.x - radius);
			pos.y = stage->rnd.Next<float>(radius, stage->mapSize.y - radius);
			for (i = 0; i < numMonsters; ++i) {
				{
					auto x = pos.x + stage->rnd.Next<float>(-radius, radius);
					auto y = pos.y + stage->rnd.Next<float>(-radius, radius);
					stage->spawners.Emplace().Emplace()->Init(stage, { x,y }, 1.f, 1.5f, [](Stage* stage_, XY const& pos_) {
						auto m = xx::MakeShared<T>();
						m->Init(stage_, pos_);
						stage_->monsters.Add(std::move(m));
					});
				}

				for (t = stage->time + delayNumFrames; stage->time < t;) {
					XX_YIELD_I(n);
				}
			}

			for (t = stage->time + groupDelayNumFrames; stage->time < t;) {
				XX_YIELD_I(n);
			}
		}

		XX_END(n);
		return 1;
	}

	/****************************************************************************************************************************************/
	/****************************************************************************************************************************************/

	template<typename T1, typename T2>
	inline void MonsterGen_Group2<T1, T2>::Init(Stage* stage_, float activeSeconds_, float destroySeconds_, int32_t numMonsters1_, int32_t numMonsters2_, float radius_, float durationSeconds_, int32_t numGroups_) {
		stage = stage_;
		activeTime = stage_->time + int32_t(Cfg::fps * activeSeconds_);
		destroyTime = stage_->time + int32_t(Cfg::fps * destroySeconds_);

		radius = radius_;
		numMonsters1 = numMonsters1_;
		numMonsters2 = numMonsters2_;
		numGroups = numGroups_;
		delayNumFrames = int32_t(Cfg::fps * durationSeconds_) / (numMonsters1_ + numMonsters2_);

		auto d = destroyTime - activeTime;
		assert(d >= delayNumFrames * numGroups_);
		groupDelayNumFrames = d / numGroups_ - delayNumFrames;
	}

	template<typename T1, typename T2>
	inline int32_t MonsterGen_Group2<T1, T2>::Update() {
		XX_BEGIN(n);

		for (j = 0; j < numGroups; ++j) {

			pos.x = stage->rnd.Next<float>(radius, stage->mapSize.x - radius);
			pos.y = stage->rnd.Next<float>(radius, stage->mapSize.y - radius);
			for (i = 0; i < numMonsters1; ++i) {
				{
					auto x = pos.x + stage->rnd.Next<float>(-radius, radius);
					auto y = pos.y + stage->rnd.Next<float>(-radius, radius);
					stage->spawners.Emplace().Emplace()->Init(stage, { x,y }, 1.f, 1.5f, [](Stage* stage_, XY const& pos_) {
						auto m = xx::MakeShared<T1>();
						m->Init(stage_, pos_);
						stage_->monsters.Add(std::move(m));
					});
				}

				for (t = stage->time + delayNumFrames; stage->time < t;) {
					XX_YIELD_I(n);
				}
			}
			for (i = 0; i < numMonsters2; ++i) {
				{
					auto x = pos.x + stage->rnd.Next<float>(-radius, radius);
					auto y = pos.y + stage->rnd.Next<float>(-radius, radius);
					stage->spawners.Emplace().Emplace()->Init(stage, { x,y }, 1.f, 1.5f, [](Stage* stage_, XY const& pos_) {
						auto m = xx::MakeShared<T2>();
						m->Init(stage_, pos_);
						stage_->monsters.Add(std::move(m));
					});
				}

				for (t = stage->time + delayNumFrames; stage->time < t;) {
					XX_YIELD_I(n);
				}
			}

			for (t = stage->time + groupDelayNumFrames; stage->time < t;) {
				XX_YIELD_I(n);
			}
		}

		XX_END(n);
		return 1;
	}

	/****************************************************************************************************************************************/
	/****************************************************************************************************************************************/

	template<typename T1, typename T2, typename T3>
	inline void MonsterGen_Group3<T1, T2, T3>::Init(Stage* stage_, float activeSeconds_, float destroySeconds_, int32_t numMonsters1_, int32_t numMonsters2_, int32_t numMonsters3_, float radius_, float durationSeconds_, int32_t numGroups_) {
		stage = stage_;
		activeTime = stage_->time + int32_t(Cfg::fps * activeSeconds_);
		destroyTime = stage_->time + int32_t(Cfg::fps * destroySeconds_);

		radius = radius_;
		numMonsters1 = numMonsters1_;
		numMonsters2 = numMonsters2_;
		numMonsters3 = numMonsters3_;
		numGroups = numGroups_;
		delayNumFrames = int32_t(Cfg::fps * durationSeconds_) / (numMonsters1_ + numMonsters2_ + numMonsters3_);

		auto d = destroyTime - activeTime;
		assert(d >= delayNumFrames * numGroups_);
		groupDelayNumFrames = d / numGroups_ - delayNumFrames;
	}

	template<typename T1, typename T2, typename T3>
	inline int32_t MonsterGen_Group3<T1, T2, T3>::Update() {
		XX_BEGIN(n);

		for (j = 0; j < numGroups; ++j) {

			pos.x = stage->rnd.Next<float>(radius, stage->mapSize.x - radius);
			pos.y = stage->rnd.Next<float>(radius, stage->mapSize.y - radius);
			for (i = 0; i < numMonsters1; ++i) {
				{
					auto x = pos.x + stage->rnd.Next<float>(-radius, radius);
					auto y = pos.y + stage->rnd.Next<float>(-radius, radius);
					stage->spawners.Emplace().Emplace()->Init(stage, { x,y }, 1.f, 1.5f, [](Stage* stage_, XY const& pos_) {
						auto m = xx::MakeShared<T1>();
						m->Init(stage_, pos_);
						stage_->monsters.Add(std::move(m));
					});
				}

				for (t = stage->time + delayNumFrames; stage->time < t;) {
					XX_YIELD_I(n);
				}
			}
			for (i = 0; i < numMonsters2; ++i) {
				{
					auto x = pos.x + stage->rnd.Next<float>(-radius, radius);
					auto y = pos.y + stage->rnd.Next<float>(-radius, radius);
					stage->spawners.Emplace().Emplace()->Init(stage, { x,y }, 1.f, 1.5f, [](Stage* stage_, XY const& pos_) {
						auto m = xx::MakeShared<T2>();
						m->Init(stage_, pos_);
						stage_->monsters.Add(std::move(m));
					});
				}

				for (t = stage->time + delayNumFrames; stage->time < t;) {
					XX_YIELD_I(n);
				}
			}
			for (i = 0; i < numMonsters3; ++i) {
				{
					auto x = pos.x + stage->rnd.Next<float>(-radius, radius);
					auto y = pos.y + stage->rnd.Next<float>(-radius, radius);
					stage->spawners.Emplace().Emplace()->Init(stage, { x,y }, 1.f, 1.5f, [](Stage* stage_, XY const& pos_) {
						auto m = xx::MakeShared<T3>();
						m->Init(stage_, pos_);
						stage_->monsters.Add(std::move(m));
					});
				}

				for (t = stage->time + delayNumFrames; stage->time < t;) {
					XX_YIELD_I(n);
				}
			}

			for (t = stage->time + groupDelayNumFrames; stage->time < t;) {
				XX_YIELD_I(n);
			}
		}

		XX_END(n);
		return 1;
	}

}
