#pragma once

namespace Game {

	inline void Map::Init() {
		// Ｂ	block
		// ｐ	player born place
		// ｍ	monster born place
		// Ｍ	monster fly target
		static std::u32string_view mapText{ UR"(
Ｂ　　　　ｍ　　　ｍ　　　ｍ　　　ｍ　　　ｍ　　　　Ｂ
Ｂ　　　　　　　　　　　　　　　　　　　　　　　　　Ｂ
Ｂ　　　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　　　Ｂ
Ｂ　　　　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　　　　Ｂ
Ｂ　　　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　　　Ｂ
Ｂ　　　　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　　　　Ｂ
Ｂ　　　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　　　Ｂ
Ｂ　　　　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　　　　Ｂ
Ｂ　　　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　　　Ｂ
Ｂ　　　　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　Ｍ　　　　Ｂ
Ｂ　　　　　　　　　　　　　　　　　　　　　　　　　Ｂ
Ｂ　　　　　　　　　　　　　　　　　　　　　　　　　Ｂ
ＢＢＢ　　　　　　　　　　　　　　　　　ＢＢＢＢＢＢＢ
ＢＢＢＢ　　　　　　　　　　　　　　　　Ｂ　　　　　Ｂ
ＢＢＢＢＢ　　　　ＢＢ　　　　　　　　　　　　　　　Ｂ
ＢＢＢＢＢＢ　　ＢＢＢＢ　ｐ　　　　　　　　　　　　Ｂ
ＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢ
)" };

		// remove first line control chars
		while (true) {
			assert(!mapText.empty());
			if (auto c = mapText[0]; c == U'\r' || c == U'\n') {
				mapText = mapText.substr(1, mapText.size() - 1);
				continue;
			}
			break;
		}

		// remove last line control chars
		while (true) {
			assert(!mapText.empty());
			if (auto c = mapText.back(); c == U'\r' || c == U'\n') {
				mapText = mapText.substr(0, mapText.size() - 1);
				continue;
			}
			break;
		}

		// calculate map width
		int32_t mapWidth{};
		for (int i = 0, e = (int)mapText.size(); i < e; ++i) {
			if (auto c = mapText[i]; c == U'\r' || c == U'\n')
				break;
			++mapWidth;
		}

		// calculate map height
		int32_t mapHeight{ 1 };
		int32_t x{};
		for (int i = 0, e = (int)mapText.size(); i < e; ++i) {
			switch (auto c = mapText[i]) {
			case U'\r':
				continue;
			case U'\n':
				assert(x == mapWidth);	// ensure every row same width
				x = 0;
				++mapHeight;
				continue;
			}
			++x;
		}

		blocks.Init(&gLooper.rdd, mapHeight, mapWidth, Cfg::unitSizei);

		// scan contents
		x = 0;
		int32_t y{};
		for (int i = 0, e = (int)mapText.size(); i < e; ++i) {
			switch (auto c = mapText[i]) {
			case U'\r':
				continue;
			case U'\n':
				x = 0;
				++y;
				continue;
			case U'Ｂ':
				blocks.Add(xx::MakeShared<Block>()->Init(XY{ x, y } * Cfg::unitSize, Cfg::unitSize));
				break;
			case U'ｐ':
				bornPlaces_Player.Emplace(x, y);
				break;
			case U'ｍ':
				bornPlaces_Monster.Emplace(x, y);
				break;
			case U'Ｍ':
				flyTargets_Monster.Emplace(x, y);
				break;
			}
			++x;
		}

		// fill block wayout data
		for (auto& o : blocks.items) {
			o->FillWayout(blocks);
		}
	}

	inline void Map::Update() {
		bgUvOffset += XY{ 0.6f, -1.f } * 10 * Cfg::frameDelay;
	}

	inline void Map::Draw(Stage* stage) const {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstanceTilingOffset)
			.Draw(gLooper.res._texid_bg_space1, 1);
		q->pos = {};
		q->anchor = 0.5f;
		q->scale = gLooper.windowSize.x / ResTpFrames::_size_bg_space1.x;
		q->radians = 0;
		q->colorplus = 0.5f;
		q->color = xx::RGBA8_White;
		q->texRect.data = ResTpFrames::_uvrect_bg_space1.data;
		q->tiling = 10;
		q->offset = bgUvOffset;

		for (auto& o : blocks.items) o->Draw(stage);
	}

	inline uint8_t const* Map::GetFlowField(XYi cri) const {
		assert(cri.x >= 0 && cri.x < blocks.numCols);
		assert(cri.y >= 0 && cri.y < blocks.numRows);
		auto ci = blocks.ColRowIndexToCellIndex(cri);
		if (!flowFields[ci]) {
			xx::Listi32<XYi> neighbors;
			neighbors.Reserve(blocks.cellsLen);
			auto d = std::make_unique_for_overwrite<uint8_t[]>(blocks.cellsLen);
			auto f = std::make_unique<uint32_t[]>(blocks.cellsLen);
			f[ci] = 1;
			neighbors.Emplace(cri);
			for (int32_t i = 0; i < neighbors.len; ++i) {
				// todo: fill neighbors & f
			}
			for (int32_t y = 0; y < blocks.numRows; ++y) {
				for (int32_t x = 0; x < blocks.numCols; ++x) {
					// todo: fill d
				}
			}
			flowFields[ci] = std::move(d);
		}
		return flowFields[ci].get();
	}

}
