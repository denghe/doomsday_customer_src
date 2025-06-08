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
ＢＢＢＢＢＢＢＢＢＢ　　　　　　　　　　　　　　　　Ｂ
Ｂ　　　Ｂ　　　　Ｂ　　　　　　　　　　ＢＢＢＢＢＢＢ
Ｂ　Ｂ　　　Ｂ　　Ｂ　　　　　　　　　　Ｂ　　　　　Ｂ
Ｂ　ＢＢＢＢＢＢＢＢ　　　　　　　　　　　　　　　　Ｂ
Ｂ　　　　　　　　　　　　ｐ　　　　　　　　　Ｂ　　Ｂ
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

		flowFields = std::make_unique<std::unique_ptr<uint8_t[]>[]>(blocks.cellsLen);
		tmp = std::make_unique_for_overwrite<uint32_t[]>(blocks.cellsLen);
		tmp2 = std::make_unique_for_overwrite<XYi[]>(blocks.cellsLen);

		// test
		//auto r = GetFlowFieldVec({23,14});
	}

	inline void Map::Update() {
		bgUvOffset += XY{ 0.6f, -1.f } * 10 * Cfg::frameDelay;
	}

	inline void Map::Draw(Stage* stage) {
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


	XX_INLINE XY Map::GetFlowFieldVec(uint8_t* ds, XYi cri) {
		static constexpr float v{ 0.7071067811865475f };
		static constexpr XY vecs[] = {
			{0,0},
			{-v,v},
			{0,1},
			{v,v},
			{-1,0},
			{0,0},
			{1,0},
			{-v,-v},
			{0,-1},
			{v,-v}
		};
		if (cri.x < 0 || cri.x >= blocks.numCols 
			|| cri.y < 0 || cri.y >= blocks.numRows) return {};
		auto ci = blocks.ColRowIndexToCellIndex(cri);
		return vecs[ds[ci]];
	}

	inline XY Map::GetFlowFieldVec(XY fromPos, float radius, XY tarPos) {
		assert(fromPos.x >= 0 && fromPos.x < blocks.gridSize.x);
		assert(fromPos.y >= 0 && fromPos.y < blocks.gridSize.y);
		auto cri = blocks.PosToColRowIndex(tarPos);
		assert(cri.x >= 0 && cri.x < blocks.numCols);
		assert(cri.y >= 0 && cri.y < blocks.numRows);
		auto bci = blocks.ColRowIndexToCellIndex(cri);
		assert(!blocks.cells[bci]);
		if (!flowFields[bci]) {
			// gen
			auto d = std::make_unique_for_overwrite<uint8_t[]>(blocks.cellsLen);
			memset(d.get(), 0, blocks.cellsLen);
			auto xys = tmp2.get();								// neighbor list
			auto ns = tmp.get();								// flow numbers
			memset(ns, 0, sizeof(uint32_t) * blocks.cellsLen);
			xys[0] = cri;										// first neighbor
			ns[bci] = 1;										// first number
			for (int32_t len = 1, i = 0; i < len; ++i) {
				cri = xys[i];
				auto ci = blocks.ColRowIndexToCellIndex(cri);
				auto n = ns[ci] + 1;
				{
					// [][][]
					// []()##
					// [][][]
					XYi ncri{ cri.x + 1, cri.y };
					if (ncri.x < blocks.numCols) {
						auto nci = blocks.ColRowIndexToCellIndex(ncri);
						if (!blocks.cells[nci] && !ns[nci]) {
							xys[len++] = ncri;
							ns[nci] = n;
						}
					}
				}
				{
					// [][][]
					// []()[]
					// []##[]
					XYi ncri{ cri.x, cri.y + 1 };
					if (ncri.y < blocks.numRows) {
						auto nci = blocks.ColRowIndexToCellIndex(ncri);
						if (!blocks.cells[nci] && !ns[nci]) {
							xys[len++] = ncri;
							ns[nci] = n;
						}
					}
				}
				{
					// [][][]
					// ##()[]
					// [][][]
					XYi ncri{ cri.x - 1, cri.y };
					if (ncri.x >= 0) {
						auto nci = blocks.ColRowIndexToCellIndex(ncri);
						if (!blocks.cells[nci] && !ns[nci]) {
							xys[len++] = ncri;
							ns[nci] = n;
						}
					}
				}
				{
					// []##[]
					// []()[]
					// [][][]
					XYi ncri{ cri.x, cri.y - 1 };
					if (ncri.y >= 0) {
						auto nci = blocks.ColRowIndexToCellIndex(ncri);
						if (!blocks.cells[nci] && !ns[nci]) {
							xys[len++] = ncri;
							ns[nci] = n;
						}
					}
				}
				{
					// [][][]
					// []()[]
					// [][]##
					XYi ncri{ cri.x + 1, cri.y + 1 };
					if (ncri.x < blocks.numCols && ncri.y < blocks.numRows) {
						auto nci = blocks.ColRowIndexToCellIndex(ncri);
						if (!blocks.cells[nci] && !ns[nci]) {
							xys[len++] = ncri;
							ns[nci] = n;
						}
					}
				}
				{
					// [][][]
					// []()[]
					// ##[][]
					XYi ncri{ cri.x - 1, cri.y + 1 };
					if (ncri.x >= 0 && ncri.y < blocks.numRows) {
						auto nci = blocks.ColRowIndexToCellIndex(ncri);
						if (!blocks.cells[nci] && !ns[nci]) {
							xys[len++] = ncri;
							ns[nci] = n;
						}
					}
				}
				{
					// ##[][]
					// []()[]
					// [][][]
					XYi ncri{ cri.x - 1, cri.y - 1 };
					if (ncri.x >= 0 && ncri.y >= 0) {
						auto nci = blocks.ColRowIndexToCellIndex(ncri);
						if (!blocks.cells[nci] && !ns[nci]) {
							xys[len++] = ncri;
							ns[nci] = n;
						}
					}
				}
				{
					// [][]##
					// []()[]
					// [][][]
					XYi ncri{ cri.x + 1, cri.y - 1 };
					if (ncri.x < blocks.numCols && ncri.y >= 0) {
						auto nci = blocks.ColRowIndexToCellIndex(ncri);
						if (!blocks.cells[nci] && !ns[nci]) {
							xys[len++] = ncri;
							ns[nci] = n;
						}
					}
				}
			}
			for (cri.y = 0; cri.y < blocks.numRows; ++cri.y) {
				for (cri.x = 0; cri.x < blocks.numCols; ++cri.x) {
					auto ci = blocks.ColRowIndexToCellIndex(cri);
					if (blocks.cells[ci]) {
						//xx::Cout(d[ci], " ");
						continue;
					}
					uint32_t n = ns[ci];
					{
						// [][][]
						// []() 6
						// [][][]
						XYi ncri{ cri.x + 1, cri.y };
						if (ncri.x < blocks.numCols) {
							auto nci = blocks.ColRowIndexToCellIndex(ncri);
							if (!blocks.cells[nci] && ns[nci] < n) {
								n = ns[nci];
								d[ci] = 6;
							}
						}
					}
					{
						// [][][]
						// []()[]
						// [] 2[]
						XYi ncri{ cri.x, cri.y + 1 };
						if (ncri.y < blocks.numRows) {
							auto nci = blocks.ColRowIndexToCellIndex(ncri);
							if (!blocks.cells[nci] && ns[nci] < n) {
								n = ns[nci];
								d[ci] = 2;
							}
						}
					}
					{
						// [][][]
						// 4 ()[]
						// [][][]
						XYi ncri{ cri.x - 1, cri.y };
						if (ncri.x >= 0) {
							auto nci = blocks.ColRowIndexToCellIndex(ncri);
							if (!blocks.cells[nci] && ns[nci] < n) {
								n = ns[nci];
								d[ci] = 4;
							}
						}
					}
					{
						// [] 8[]
						// []()[]
						// [][][]
						XYi ncri{ cri.x, cri.y - 1 };
						if (ncri.y >= 0) {
							auto nci = blocks.ColRowIndexToCellIndex(ncri);
							if (!blocks.cells[nci] && ns[nci] < n) {
								n = ns[nci];
								d[ci] = 8;
							}
						}
					}
					{
						// [][][]
						// []()[]
						// [][] 3
						XYi ncri{ cri.x + 1, cri.y + 1 };
						if (ncri.x < blocks.numCols && ncri.y < blocks.numRows) {
							auto nci = blocks.ColRowIndexToCellIndex(ncri);
							if (!blocks.cells[nci] && ns[nci] < n) {
								n = ns[nci];
								d[ci] = 3;
							}
						}
					}
					{
						// [][][]
						// []()[]
						// 1 [][]
						XYi ncri{ cri.x - 1, cri.y + 1 };
						if (ncri.x >= 0 && ncri.y < blocks.numRows) {
							auto nci = blocks.ColRowIndexToCellIndex(ncri);
							if (!blocks.cells[nci] && ns[nci] < n) {
								n = ns[nci];
								d[ci] = 1;
							}
						}
					}
					{
						// 7 [][]
						// []()[]
						// [][][]
						XYi ncri{ cri.x - 1, cri.y - 1 };
						if (ncri.x >= 0 && ncri.y >= 0) {
							auto nci = blocks.ColRowIndexToCellIndex(ncri);
							if (!blocks.cells[nci] && ns[nci] < n) {
								n = ns[nci];
								d[ci] = 7;
							}
						}
					}
					{
						// [][] 9
						// []()[]
						// [][][]
						XYi ncri{ cri.x + 1, cri.y - 1 };
						if (ncri.x < blocks.numCols && ncri.y >= 0) {
							auto nci = blocks.ColRowIndexToCellIndex(ncri);
							if (!blocks.cells[nci] && ns[nci] < n) {
								n = ns[nci];
								d[ci] = 9;
							}
						}
					}
					//xx::Cout(d[ci], " ");
				}
				//xx::CoutN();
			}
			flowFields[bci] = std::move(d);
		}
		auto ds = flowFields[bci].get();

#if 0
		return GetFlowFieldVec(ds, blocks.PosToColRowIndex(fromPos));
#else
		// calc move dir ( 4 point lerp bilinear )
		// L left T top R right B bottom H horizontal V vertical
		auto posLT = fromPos - radius;
		auto posRB = fromPos + radius;
		if (posLT.x < 0) posLT.x = 0;
		if (posLT.y < 0) posLT.y = 0;
		if (posRB.x >= blocks.gridSize.x) posRB.x = blocks.gridSize.x - 1.f;
		if (posRB.y >= blocks.gridSize.y) posRB.y = blocks.gridSize.y - 1.f;
		auto criLT = blocks.PosToColRowIndex(posLT);
		auto criRB = blocks.PosToColRowIndex(posRB);

		auto x = (criLT.x + 1) * blocks.cellSize;
		auto y = (criLT.y + 1) * blocks.cellSize;

		auto hL = x - posLT.x;
		auto hR = posRB.x - x;
		auto vT = y - posLT.y;
		auto vB = posRB.y - y;

		auto vecLT = GetFlowFieldVec(ds, criLT);
		auto vecRT = GetFlowFieldVec(ds, { criRB.x, criLT.y });
		auto vecLB = GetFlowFieldVec(ds, { criLT.x, criRB.y });
		auto vecRB = GetFlowFieldVec(ds, criRB);
		
		//auto _1_r2 = 1.f / radius * 2.f;
		//auto vecT = vecLT * hL * _1_r2 + vecRT * hR * _1_r2;
		//auto vecB = vecLB * hL * _1_r2 + vecRT * hR * _1_r2;
		//auto vec = vecT * vT * _1_r2 + vecB * vB * _1_r2;

		auto vecT = vecLT * hL + vecRT * hR;
		auto vecB = vecLB * hL + vecRT * hR;
		auto v = vecT * vT + vecB * vB;
		if (auto mag2 = v.x * v.x + v.y * v.y; mag2 > 0) {
			auto mag = std::sqrtf(mag2);
			auto r = v / mag;
#if 0
			static constexpr float limit{ 0.3f };
			// limit r from -1 ~ -limit || limit ~ 1
			if (r.x < 0 && r.x > -limit) {
				r.x = -limit;
			}
			else if (r.x > 0 && r.x < limit) {
				r.x = limit;
			}
			if (r.y < 0 && r.y > -limit) {
				r.y = -limit;
			}
			else if (r.y > 0 && r.y < limit) {
				r.y = limit;
			}
#endif
			return r;
		}
		else return {};
#endif
	}
}
