#pragma once

namespace Game {
	template<typename T>
	int32_t MonsterGen_Generic<T>::Update() {
		//CreateFromRect<T>();
		//CreateFromDoughnut<T>();
		CreateFromFullMap<T>();
		return 0;
	}



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




	inline void MonsterGen_2xHamburger::Init(Stage* stage_, float activeSeconds_, float destroySeconds_, int32_t numGroups_) {
		stage = stage_;
		activeTime = stage_->time + int32_t(Cfg::fps * activeSeconds_);
		destroyTime = stage_->time + int32_t(Cfg::fps * destroySeconds_);
		numGroups = numGroups_;

		auto d = destroyTime - activeTime;
		assert(d >= cStepNumFrames * numGroups_);
		groupDelayNumFrames = d / numGroups_ - cStepNumFrames;
	}

	inline int32_t MonsterGen_2xHamburger::Update() {
		XX_BEGIN(n);

		for (j = 0; j < numGroups; ++j) {

			pos.x = stage->rnd.Next<float>(cRadius, stage->mapSize.x - cRadius);
			pos.y = stage->rnd.Next<float>(cRadius, stage->mapSize.y - cRadius);
			for (i = 0; i < cNumMonsters; ++i) {
				{
					auto x = pos.x + stage->rnd.Next<float>(-cRadius, cRadius);
					auto y = pos.y + stage->rnd.Next<float>(-cRadius, cRadius);
					stage->spawners.Emplace().Emplace()->Init(stage, { x,y }, 1.f, 1.5f, [](Stage* stage_, XY const& pos_) {
						auto m = xx::MakeShared<Monster_Hamburger>();
						m->Init(stage_, pos_);
						stage_->monsters.Add(std::move(m));
					});
				}

				for (t = stage->time + cStepNumFrames; stage->time < t;) {
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
