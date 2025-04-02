﻿#pragma once

namespace Game {

	struct Ground {
		Stage* stage{};
		XYi gridSize{};
		xx::Rnd rnd;
		std::unique_ptr<uint8_t[]> colors;

		void Init(Stage* stage_, XY mapSize);
		void Draw();
	};

}
