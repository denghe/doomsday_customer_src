#pragma once

namespace Game {

	struct Stage1;
	struct Ground {
		Stage1* owner{};
		XYi gridSize{};
		XY scale{ 1,1 };
		xx::Rnd rnd;
		std::unique_ptr<uint8_t[]> colors;

		void Init(Stage1* owner_, XYi gridSize_);
		void Draw();
	};

}
