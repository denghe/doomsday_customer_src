#pragma once

namespace Game {
	inline void MonsterFormation::Init(Stage* stage_) {
		assert(!stage);
		stage = stage_;
		auto& fts = stage_->map->flyTargets_Monster;

		for (auto& crIdx : fts) {
			poss.Emplace(IdxToPos(crIdx));
		}

		assert(poss.len);
		shufflePoss.AddRange(poss.buf, poss.len);
		count = poss.len;
	}

	inline void MonsterFormation::Update() {
		static constexpr auto step{ Cfg::unitSize * 2.f / (Cfg::fps * 2.f) };
		XX_BEGIN(_n);
		for (; offset.x > -Cfg::unitSize * 2.f; offset.x -= step) {
			XX_YIELD(_n);
		}
		for (; offset.x < Cfg::unitSize * 2.f; offset.x += step) {
			XX_YIELD(_n);
		}
		XX_YIELD_TO_BEGIN(_n);
		XX_END(_n);
	}

	XX_INLINE XY MonsterFormation::IdxToPos(XYi crIdx) {
		return crIdx * Cfg::unitSize + (Cfg::unitSize * 0.5f);
	}

	XX_INLINE XY MonsterFormation::GetPos(int32_t idx) {
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

		/*
Ｂ　　　　ｍ　　　ｍ　　　ｍ　　　ｍ　　　ｍ　　　　Ｂ
Ｂ　　　　　　　　　　　　　　　　　　　　　　　　　Ｂ
Ｂ　　　０　１　２　３　４　５　６　７　８　９　　　Ｂ	0 ~ 9
Ｂ　　　　０　１　２　３　４　５　６　７　８　　　　Ｂ	10 ~ 18
Ｂ　　　０　１　２　３　４　５　６　７　８　９　　　Ｂ	19 ~ 28
Ｂ　　　　０　１　２　３　４　５　６　７　８　　　　Ｂ	29 ~ 37
Ｂ　　　０　１　２　３　４　５　６　７　８　９　　　Ｂ	38 ~ 47
Ｂ　　　　０　１　２　３　４　５　６　７　８　　　　Ｂ	48 ~ 56
Ｂ　　　０　１　２　３　４　５　６　７　８　９　　　Ｂ	57 ~ 66
Ｂ　　　　０　１　２　３　４　５　６　７　８　　　　Ｂ	67 ~ 75
Ｂ　　　　　　　　　　　　　　　　　　　　　　　　　Ｂ
Ｂ　　　　　　　　　　　　　　　　　　　　　　　　　Ｂ
ＢＢＢ　　　　　　　　　　　　　　　　　ＢＢＢＢＢＢＢ
		*/

	/**************************************************************************/
	/**************************************************************************/
	// MonsterFormation_1

	inline void MonsterFormation_1::Init(Stage* stage_) {
		assert(!stage);
		stage = stage_;
		auto& fts = stage_->map->flyTargets_Monster;

		poss.Emplace(IdxToPos(fts[29 + 4]));
		poss.Emplace(IdxToPos(fts[48 + 4]));
		poss.Emplace(IdxToPos(fts[67 + 4]));

		shufflePoss.AddRange(poss.buf, poss.len);
		count = poss.len;
	}

	inline void MonsterFormation_1::Update() {
		offset.x = stage->player->pos.x - poss[0].x;
	}

}
