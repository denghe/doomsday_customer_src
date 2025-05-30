#pragma once

namespace Game {
	inline void MonsterFormation::Init(Stage* stage_) {
		assert(!stage);
		stage = stage_;
		for (auto& crIdx : stage_->map->flyTargets_Monster) {
			auto pos = crIdx * Cfg::unitSize + (Cfg::unitSize * 0.5f);
			poss.Emplace(pos);
		}
		assert(poss.len);
		shufflePoss.AddRange(poss.buf, poss.len);
		count = poss.len;
	}

	inline void MonsterFormation::Update() {
		static constexpr auto step{ Cfg::unitSize / (Cfg::fps * 2.f) };
		XX_BEGIN(_n);
		for (; offset.x > -Cfg::unitSize; offset.x -= step) {
			XX_YIELD(_n);
		}
		for (; offset.x < Cfg::unitSize; offset.x += step) {
			XX_YIELD(_n);
		}
		XX_YIELD_TO_BEGIN(_n);
		XX_END(_n);
	}

	inline XY MonsterFormation::GetPos(int32_t idx) {
		return poss[idx] + offset;
	}

	inline void MonsterFormation::FillShufflePoss() {
		auto& rnd = stage->rnd;
		auto len = shufflePoss.len;
		auto buf = shufflePoss.buf;
		for (int32_t i = 0, tar = 1; ++i != len; ++tar) {
			if (int32_t offset = rnd.Next(tar); offset != tar) {
				std::swap(buf[i], buf[offset]);
			}
		}
	}
}
