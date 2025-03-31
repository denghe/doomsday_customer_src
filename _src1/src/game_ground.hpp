#pragma once

namespace Game {

	inline void Ground::Init(Stage1* owner_, XY mapSize) {
		owner = owner_;
		gridSize = mapSize / ResTpFrames::_size_ground_cell_;
		if (gridSize.x & 0b11) {
			gridSize.x = (gridSize.x + 4) & ~0b11;
		}
		colors = std::make_unique_for_overwrite<uint8_t[]>(gridSize.x * gridSize.y);
		rnd.SetSeed(12345678901234567890L);
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
		auto& camera = owner->camera;

		XY halfSize{ gLooper.windowSize.x * camera.zoom * 0.5f, gLooper.windowSize.y * camera.zoom * 0.5f };
		auto leftTopPos = camera.original - halfSize;
		auto rightBottomPos = camera.original + halfSize;

		auto rowFrom = int32_t(leftTopPos.y / ResTpFrames::_size_ground_cell_.y);
		if (rowFrom < 0) {
			rowFrom = 0;
		}
		auto rowTo = int32_t(rightBottomPos.y / ResTpFrames::_size_ground_cell_.y) + 1;
		if (rowTo > gridSize.y) {
			rowTo = gridSize.y;
		}
		auto colFrom = int32_t(leftTopPos.x / ResTpFrames::_size_ground_cell_.x);
		if (colFrom < 0) {
			colFrom = 0;
		}
		auto colTo = int32_t(rightBottomPos.x / ResTpFrames::_size_ground_cell_.x) + 1;
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
				xx::XY pos{ colIdx * ResTpFrames::_size_ground_cell_.x, rowIdx * ResTpFrames::_size_ground_cell_.y };
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
