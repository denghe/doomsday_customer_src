#pragma once

namespace Game {

	inline void Ground::Init(Stage1* owner_, XYi gridSize_) {
		owner = owner_;
		gridSize = gridSize_;
	}

	inline void Ground::Draw() {
		auto& camera = owner->camera;
		auto wndSize = gLooper.windowSize;

		int32_t rowFrom{}, rowTo{}, colFrom{}, colTo{};

		int32_t halfNumRows = int32_t(wndSize.y / camera.scale) / cCellSize.y / 2;
		int32_t posRowIndex = (int32_t)camera.original.y / cCellSize.y;
		rowFrom = posRowIndex - halfNumRows - 1;
		rowTo = posRowIndex + halfNumRows + 3;
		if (rowFrom < 0) {
			rowFrom = 0;
		}
		if (rowTo > gridSize.y) {
			rowTo = gridSize.y;
		}

		int32_t halfNumCols = int32_t(wndSize.x / camera.scale) / cCellSize.x / 2;
		int32_t posColIndex = (int32_t)camera.original.x / cCellSize.x;
		colFrom = posColIndex - halfNumCols - 1;
		colTo = posColIndex + halfNumCols + 3;
		if (colFrom < 0) {
			colFrom = 0;
		}
		if (colTo > gridSize.x) {
			colTo = gridSize.x;
		}

		auto numRows = rowTo - rowFrom;
		auto numCols = colTo - colFrom;

		auto& frame = *gLooper.res.ground_cell;
		auto buf = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(frame.tex->GetValue(), numCols * numRows);

		for (int32_t rowIdx = rowFrom; rowIdx < rowTo; ++rowIdx) {
			for (int32_t colIdx = colFrom; colIdx < colTo; ++colIdx) {
				auto& q = buf[numCols * (rowIdx - rowFrom) + (colIdx - colFrom)];
				xx::XY pos{ colIdx * cCellSize.x, rowIdx * cCellSize.y };
				q.pos = owner->camera.ToGLPos(pos);
				q.anchor = { 0, 1 };
				q.scale = scale * Cfg::globalScale * owner->camera.scale;
				q.radians = 0;
				q.colorplus = 1;
				q.color = xx::RGBA8_White;
				q.texRect.data = frame.textureRect.data;
			}
		}

	}
}
