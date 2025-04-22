#pragma once

namespace Game {
	template<typename T>
	int32_t MonsterGen_Generic<T>::Update() {
		//CreateFromRect<T>();
		//CreateFromDoughnut<T>();
		CreateFromFullMap<T>();
		return 0;
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
