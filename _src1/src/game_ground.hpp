#pragma once

namespace Game {

	inline void Ground::Init(Stage1* owner_, XYi gridSize_) {
		owner = owner_;
		gridSize = gridSize_;
		colors = std::make_unique_for_overwrite<uint8_t[]>(gridSize_.x * gridSize_.y);
		rnd.SetSeed(12345678901234567890L);
		for (int32_t rowIdx = 0; rowIdx < gridSize_.x; ++rowIdx) {
			for (int32_t colIdx = 0; colIdx < gridSize_.y; ++colIdx) {
				auto idx = gridSize_.x * rowIdx + colIdx;
				colors[idx] = 255 - 63 + ((uint8_t)rnd.Get() & 63);
			}
		}
	}

	inline void Ground::Draw() {
		auto& camera = owner->camera;

		XY halfSize{ gLooper.windowSize.x * camera.zoom * 0.5f, gLooper.windowSize.y * camera.zoom * 0.5f };
		auto leftTopPos = camera.original - halfSize;
		auto rightBottomPos = camera.original + halfSize;

		auto rowFrom = int32_t(leftTopPos.y / cCellSize.y);
		if (rowFrom < 0) {
			rowFrom = 0;
		}
		auto rowTo = int32_t(rightBottomPos.y / cCellSize.y) + 1;
		if (rowTo > gridSize.y) {
			rowTo = gridSize.y;
		}
		auto colFrom = int32_t(leftTopPos.x / cCellSize.x);
		if (colFrom < 0) {
			colFrom = 0;
		}
		auto colTo = int32_t(rightBottomPos.x / cCellSize.x) + 1;
		if (colTo > gridSize.x) {
			colTo = gridSize.x;
	}
		if (rowTo == rowFrom || colTo == colFrom) return;

		auto numRows = rowTo - rowFrom;
		auto numCols = colTo - colFrom;

		auto& frame = *gLooper.res.ground_cell;
		auto buf = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(frame.tex->GetValue(), numCols * numRows);

		for (int32_t rowIdx = rowFrom; rowIdx < rowTo; ++rowIdx) {
			for (int32_t colIdx = colFrom; colIdx < colTo; ++colIdx) {
				auto& q = buf[numCols * (rowIdx - rowFrom) + (colIdx - colFrom)];
				xx::XY pos{ colIdx * cCellSize.x, rowIdx * cCellSize.y };
				q.pos = camera.ToGLPos(pos);
				q.anchor = { 0, 1 };
				q.scale = scale * Cfg::globalScale * camera.scale;
				q.radians = 0;
				q.colorplus = 1;
				q.color = { 255, 255, 255, colors[gridSize.x * rowIdx + colIdx]};
				q.texRect.data = frame.textureRect.data;
			}
		}

	}
}
