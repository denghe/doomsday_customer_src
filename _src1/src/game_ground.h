#pragma once

namespace Game {

	struct Ground {
		static constexpr float scale{ 1 };
		static constexpr XY size{ ResTpFrames::_size_ground_cell_ * scale };
		Stage* stage{};
		XYi gridSize{};
		xx::Rnd rnd;
		std::unique_ptr<uint8_t[]> colors;

		void Init(Stage* stage_, XY mapSize);
		void Draw();
	};

}
