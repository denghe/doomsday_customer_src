#pragma once

namespace Game {

	struct Ground {
		Stage* stage{};
		xx::Ref<xx::Frame> frame;
		XY scale{};
		xx::RGB8 color{};
		XY size{};
		XYi gridSize{};
		xx::Rnd rnd;
		std::unique_ptr<uint8_t[]> colors;

		void Init(Stage* stage_, XY mapSize, xx::Ref<xx::Frame> const& frame_, XY scale_ = 1.f, xx::RGB8 color_ = xx::RGB8_White);
		void Draw();
	};

}
