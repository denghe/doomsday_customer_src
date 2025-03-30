#pragma once

namespace Game {

	struct Stage1;
	struct Ground {
		static constexpr XYi cCellSize{ 85, 85 };	// same as res size

		Stage1* owner{};
		XYi gridSize{};
		XY scale{ 1,1 };
		// todo: diff colors ??

		void Init(Stage1* owner_, XYi gridSize_);
		void Draw();
	};

}
