#pragma once

namespace Game {

	// map elements:
	// Ｂ	block
	// ｐ	player born place
	// ｍ	monster born place
	// Ｍ	monster fly target
	inline void Map::Init(Stage* stage_, std::u32string_view mapText) {
		this->stage = stage_;

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
				flyTargets_Monster.Emplace(IdxToPos({ x, y }));
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

		ShuffleFlyTargets();

		auto ms = blocks.gridSize.As<float>();
		cameraOriginal = { ms.x * 0.5f, ms.y - Cfg::height_2 };
	}

	inline int32_t Map::Update() {
		bgUvOffset += XY{ 0.6f, -1.f } * 10 * Cfg::frameDelay;
		return 0;
	}

	inline void Map::Draw() {
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

	inline void Map::DrawLight(float colorPlus_) {
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



	XX_INLINE XY Map::IdxToPos(XYi crIdx) {
		return crIdx * Cfg::unitSize + (Cfg::unitSize * 0.5f);
	}

	XX_INLINE XY Map::GetFlyTargetPos(int32_t idx) {
		return flyTargets_Monster[idx];
	}

	XX_INLINE XY Map::GetFlyTargetPosWithOffset(int32_t idx) {
		return flyTargets_Monster[idx] + offset;
	}

	inline void Map::ShuffleFlyTargets() {
		auto& rnd = stage->rnd;
		auto len = flyTargets_Monster.len;
		auto buf = flyTargets_Monster.buf;
		for (int32_t i = 0, tar = 1; ++i != len; ++tar) {
			if (int32_t offset = rnd.Next(tar); offset != tar) {
				std::swap(buf[i], buf[offset]);
			}
		}
	}

	inline int32_t Map::GenerateMonster() {
		return GenerateMonster_core<Monster>();
	}

	template<typename M>
	XX_INLINE int32_t Map::GenerateMonster_core() {
		XX_BEGIN(_1n);
		for (_1a = 0; _1a < flyTargets_Monster.len; ++_1a) {
			{
				// make monster
				auto idx = stage->rnd.Next<int32_t>(bornPlaces_Monster.len);
				auto bornPos = IdxToPos(bornPlaces_Monster[idx]);
				auto m = xx::MakeShared<M>();
				m->Init(stage, bornPos, _1a);
				stage->monsters.Add(std::move(m));
			}

			// sleep
			for (_1b = stage->time + int32_t(Cfg::fps * 0.01f); stage->time < _1b;) {
				XX_YIELD_I(_1n);
			}
		}
		XX_END(_1n);
		return 1;
	}



	/****************************************************************************/
	/****************************************************************************/

	inline void Map_1::Init(Stage* stage_) {
		this->Map::Init(stage_, UR"(
Ｂ　　ｍｍｍｍｍｍｍｍｍｍｍｍｍｍｍｍｍｍｍｍｍ　　Ｂ
Ｂ　　　　　　　　　　　　　　　　　　　　　　　　　Ｂ
Ｂ　　　　　　　　　　　　　　　　　　　　　　　　　Ｂ
Ｂ　　　　　　　　　　　　　　　　　　　　　　　　　Ｂ
Ｂ　　　　　　　　　　　　　　　　　　　　　　　　　Ｂ
Ｂ　　　　　　　　　　　　　　　　　　　　　　　　　Ｂ
Ｂ　　　　　　　　　　　　　　　　　　　　　　　　　Ｂ
Ｂ　　　　　　　　　　　　　　　　　　　　　　　　　Ｂ
Ｂ　　　　Ｍ　　　Ｍ　　　Ｍ　　　Ｍ　　　Ｍ　　　　Ｂ
Ｂ　　　　　　　　　　　　　　　　　　　　　　　　　Ｂ
Ｂ　　　　　　　　　　　　　　　　　　　　　　　　　Ｂ
Ｂ　　　　　　　　　　　　　　　　　　　　　　　　　Ｂ
Ｂ　　　　　　　　　　　　　　　　　　　　　　　　　Ｂ
Ｂ　　　　　　　　　　　　　　　　　　　　　　　　　Ｂ
Ｂ　　　　　　　　　　　　　　　　　　　　　　　　　Ｂ
Ｂ　　　　　　　　　　　　　　　　　　　　　　　　　Ｂ
Ｂ　　　　　　　　　　　　　　　　　　　　　　　　　Ｂ
Ｂ　　　　　　　　　　　　ｐ　　　　　　　　　　　　Ｂ
ＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢＢ
)");
	}

	inline int32_t Map_1::GenerateMonster() {
		return GenerateMonster_core<Monster_1>();
	}

	inline int32_t Map_1::Update() {
		this->Map::Update();
		offset.x = (stage->player->pos.x - cameraOriginal.x) * 0.2f;
		return 0;
	}

	/****************************************************************************/
	/****************************************************************************/

	inline void Map_2::Init(Stage* stage_) {
		this->Map::Init(stage_, UR"(
Ｂ　　ｍｍｍｍｍｍｍｍｍｍｍｍｍｍｍｍｍｍｍｍｍ　　Ｂ
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
)");
	}

	inline int32_t Map_2::Update() {
		this->Map::Update();
		static constexpr auto step{ Cfg::unitSize * 2.f / (Cfg::fps * 2.f) };
		XX_BEGIN(_n);
		while (true) {
			for (; offset.x > -Cfg::unitSize * 2.f; offset.x -= step) {
				XX_YIELD_I(_n);
			}
			for (; offset.x < Cfg::unitSize * 2.f; offset.x += step) {
				XX_YIELD_I(_n);
			}
		}
		XX_END(_n);
		return 0;
	}

}
