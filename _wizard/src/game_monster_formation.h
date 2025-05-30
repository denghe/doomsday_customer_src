#pragma once

namespace Game {

	struct MonsterFormation {
		Stage* stage{};
		xx::Listi32<XY> poss;
		xx::Listi32<XY> shufflePoss;
		XY offset{};
		int32_t count{};	// cache: poss.len
		int32_t _n{};

		void Init(Stage* stage_);
		void Update();
		XY GetPos(int32_t idx);
		void FillShufflePoss();
	};

}
