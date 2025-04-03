#pragma once

namespace Game {

	struct Ground {
		static constexpr float cScale{ 3 };
		static constexpr xx::RGB8 cColor{ 100,100,100 };
		static constexpr XY cSize{ ResTpFrames::_size_ground_cell_ * cScale };
		Stage* stage{};
		XYi gridSize{};
		xx::Rnd rnd;
		std::unique_ptr<uint8_t[]> colors;

		void Init(Stage* stage_, XY mapSize);
		void Draw();
	};

}
