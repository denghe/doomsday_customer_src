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
		virtual void Update();
		static XY IdxToPos(XYi idx);
		XY GetPos(int32_t idx);
		void FillShufflePoss();
		virtual ~MonsterFormation() {}
	};


	struct MonsterFormation_1 : MonsterFormation {
		void Init(Stage* stage_);
		virtual void Update();
	};
}
