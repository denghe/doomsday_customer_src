#pragma once

namespace Game {

	inline void Ground::Init(Stage* stage_, XY mapSize, xx::Ref<xx::Frame> const& frame_, XY scale_, xx::RGB8 color_) {
		stage = stage_;
		frame = frame_;
		scale = scale_;
		color = color_;
		size = { frame_->textureRect.w * scale_.x, frame_->textureRect.h * scale_.y };
		gridSize = mapSize / size;
		if (gridSize.x & 0b11) {
			gridSize.x = (gridSize.x + 4) & ~0b11;
		}
		colors = std::make_unique_for_overwrite<uint8_t[]>(gridSize.x * gridSize.y);
		rnd.SetSeed(12345678901234567890UL);
		for (int32_t rowIdx = 0; rowIdx < gridSize.x; ++rowIdx) {
			for (int32_t colIdx = 0; colIdx < gridSize.y; colIdx += 4) {
				auto idxBase = gridSize.x * rowIdx;
				auto r = rnd.Get();
				auto p = (uint8_t*)&r;
				colors[idxBase + colIdx + 0] = 255 - 63 + (p[0] & 63);
				colors[idxBase + colIdx + 1] = 255 - 63 + (p[1] & 63);
				colors[idxBase + colIdx + 2] = 255 - 63 + (p[2] & 63);
				colors[idxBase + colIdx + 3] = 255 - 63 + (p[3] & 63);
			}
		}
	}

	inline void Ground::Draw() {
		auto& camera = stage->camera;

		XY halfSize{ gLooper.windowSize.x / camera.scale * 0.5f, gLooper.windowSize.y / camera.scale * 0.5f };
		auto leftTopPos = camera.original - halfSize;
		auto rightBottomPos = camera.original + halfSize;

		auto rowFrom = int32_t(leftTopPos.y / size.y);
		if (rowFrom < 0) {
			rowFrom = 0;
		}
		auto rowTo = int32_t(rightBottomPos.y / size.y) + 1;
		if (rowTo > gridSize.y) {
			rowTo = gridSize.y;
		}
		auto colFrom = int32_t(leftTopPos.x / size.x);
		if (colFrom < 0) {
			colFrom = 0;
		}
		auto colTo = int32_t(rightBottomPos.x / size.x) + 1;
		if (colTo > gridSize.x) {
			colTo = gridSize.x;
	}
		if (rowTo == rowFrom || colTo == colFrom) return;

		auto numRows = rowTo - rowFrom;
		auto numCols = colTo - colFrom;

		auto buf = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(frame->tex, numCols * numRows);
		auto rectData = frame->textureRect.data;
		auto s = camera.scale * scale;
		auto size_2 = size * 0.5f;
		for (int32_t rowIdx = rowFrom; rowIdx < rowTo; ++rowIdx) {
			for (int32_t colIdx = colFrom; colIdx < colTo; ++colIdx) {
				auto& q = buf[numCols * (rowIdx - rowFrom) + (colIdx - colFrom)];
				xx::XY pos{ colIdx * size.x, rowIdx * size.y };
				q.pos = camera.ToGLPos(pos + size_2);
				q.anchor = { .5f, .5f };
				q.scale = s;
				q.radians = 0;
				q.colorplus = 1;
				q.color = { color.r, color.g, color.b, colors[gridSize.x * rowIdx + colIdx]};
				q.texRect.data = rectData;
			}
		}

	}
}
