#pragma once

namespace Game {

	inline void Ground::Draw(xx::Listi32<std::pair<float, Game::Drawable*>>& yd, XY areaMin, XY areaMax) {
		for (auto e = grasses.len, i = 0; i < e; ++i) {
			auto& o = grasses[i];
			if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
			yd.Emplace(o->pos.y, o.pointer);
		}
	}

}

/**************************************************************************************/
/**************************************************************************************/

namespace Game {

	inline void Ground1::Init(Stage* stage_, xx::Ref<xx::Frame> const& frame_, XY scale_, xx::RGB8 color_) {
		stage = stage_;
		frame = frame_;
		scale = scale_;
		color = color_;
		size = { frame_->textureRect.w * scale_.x, frame_->textureRect.h * scale_.y };
		gridSize = stage->mapSize / size;
		if (gridSize.x & 0b11) {
			gridSize.x = (gridSize.x + 4) & ~0b11;
		}
		colors = std::make_unique_for_overwrite<uint8_t[]>(gridSize.x * gridSize.y);
		auto& rnd = stage->rnd;
		for (int32_t rowIdx = 0; rowIdx < gridSize.y; ++rowIdx) {
			for (int32_t colIdx = 0; colIdx < gridSize.x; colIdx += 4) {
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

	inline void Ground1::AddSomeGrass(float ratio, int32_t typeId) {
		static constexpr xx::FromTo<float> scaleRange{ 0.3f, 1.f };
		static constexpr xx::FromTo<float> swingRange{ -0.1f, 0.1f };

		XY groundSize{ gridSize * size };
		grasses.Reserve(100000);
		xx::Ref<xx::Frame> f;
		XY s{};
		auto& rnd = stage->rnd;

		if (typeId == 0) {
			for (int i = 0; i < stage->gridSize.x * stage->gridSize.y * ratio; ++i) {
				XY pos{ rnd.Next<float>(0, groundSize.x), rnd.Next<float>(0, groundSize.y) };
				if (rnd.Next<float>() > 0.2f) {
					f = gLooper.res.env_grass_[2];
					s = 1;
				}
				else {
					f = gLooper.res.env_grass_[rnd.Next<uint32_t>(2)];
					s.x = rnd.Next<float>(scaleRange.from, scaleRange.to);
					s.y = rnd.Next<float>(scaleRange.from, scaleRange.to);
				}
				grasses.Emplace().Emplace<EnvGrass>()->Init(stage, std::move(f), pos, s, swingRange);
			}
		}
		else if (typeId == 1) {
			for (int i = 0; i < stage->gridSize.x * stage->gridSize.y * ratio; ++i) {
				XY pos{ rnd.Next<float>(0, groundSize.x), rnd.Next<float>(0, groundSize.y) };
				f = gLooper.res.env_grass_[2];
				s = 1;
				grasses.Emplace().Emplace<EnvGrass>()->Init(stage, std::move(f), pos, s, swingRange);
			}
		}
		else {	// typeId == 2
			for (int i = 0; i < stage->gridSize.x * stage->gridSize.y * ratio; ++i) {
				XY pos{ rnd.Next<float>(0, groundSize.x), rnd.Next<float>(0, groundSize.y) };
				f = gLooper.res.env_grass_[rnd.Next<uint32_t>(2)];
				s.x = rnd.Next<float>(scaleRange.from, scaleRange.to);
				s.y = rnd.Next<float>(scaleRange.from, scaleRange.to);
				grasses.Emplace().Emplace<EnvGrass>()->Init(stage, std::move(f), pos, s, swingRange);
			}
		}

		std::sort(grasses.buf, grasses.buf + grasses.len
			, [](auto& a, auto& b) { return a->pos.y < b->pos.y; });
	}

	inline void Ground1::Update() {
		for (auto i = grasses.len - 1; i >= 0; --i) {
			auto& o = grasses[i];
			if (o->Update()) {
				grasses.SwapRemoveAt(i);
			}
		}
	}

	inline void Ground1::Clear() {
		grasses.Clear();
	}


	inline void Ground1::Draw() {
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

/**************************************************************************************/
/**************************************************************************************/

namespace Game {

	inline void Ground2::Init(Stage* stage_) {
		stage = stage_;

		// map logic size
		xx::XYi miniMapSize{ stage->gridSize };

		// tex aligned size
		xx::XYi miniTexSize{ (miniMapSize.x + 7) & ~7u, (miniMapSize.y + 7) & ~7u };

		// malloc tex mem
		auto d = std::make_unique<std::array<float, 4>[]>(miniTexSize.x * miniTexSize.y);

		// scale 9 map:
		// 00 10 20
		// 01 11 21
		// 02 12 22

		// fill tex mem
		for (int y = 0; y < miniMapSize.y; ++y) {
			float v{ 1 };
			if (y == 0) v = 0; else if (y == miniMapSize.y - 1) v = 2;
			for (int x = 0; x < miniMapSize.x; ++x) {
				float u{ 1 };
				if (x == 0) u = 0; else if (x == miniMapSize.x - 1) u = 2;
				d[y * miniTexSize.x + x] = { u, v, 1, 0 };
			}
		}

		// copy & create tex
		tex.Emplace(xx::LoadGLTiledTexture(d.get(), miniTexSize.x, miniTexSize.y, miniMapSize));
	}

	inline void Ground2::AddSomeGrass(float ratio, int32_t typeId) {
		// create some grass
		static constexpr xx::FromTo<float> scaleRange{ 0.3f, 1.f };
		static constexpr xx::FromTo<float> swingRange{ -0.1f, 0.1f };

		XY groundSize{ stage->mapSize };
		grasses.Reserve(100000);
		xx::Ref<xx::Frame> f;
		XY s{};
		auto& rnd = stage->rnd;

		if (typeId == 0) {
			for (int i = 0; i < stage->gridSize.x * stage->gridSize.y * ratio; ++i) {
				XY pos{ rnd.Next<float>(0, groundSize.x), rnd.Next<float>(0, groundSize.y) };
				if (rnd.Next<float>() > 0.2f) {
					f = gLooper.res.env_grass1_[rnd.Next<uint32_t>(ResTpFrames::_countof_env_grass1_)];
					s = 1;
				}
				else {
					f = gLooper.res.env_grass2_[rnd.Next<uint32_t>(ResTpFrames::_countof_env_grass2_)];
					s.x = rnd.Next<float>(scaleRange.from, scaleRange.to);
					s.y = rnd.Next<float>(scaleRange.from, scaleRange.to);
				}
				grasses.Emplace().Emplace<EnvGrass>()->Init(stage, std::move(f), pos, s, swingRange);
			}
		}
		else if (typeId == 1) {
			for (int i = 0; i < stage->gridSize.x * stage->gridSize.y * ratio; ++i) {
				XY pos{ rnd.Next<float>(0, groundSize.x), rnd.Next<float>(0, groundSize.y) };
				f = gLooper.res.env_grass1_[rnd.Next<uint32_t>(ResTpFrames::_countof_env_grass1_)];
				s = 1;
				grasses.Emplace().Emplace<EnvGrass>()->Init(stage, std::move(f), pos, s, swingRange);
			}
		}
		else {
			for (int i = 0; i < stage->gridSize.x * stage->gridSize.y * ratio; ++i) {
				XY pos{ rnd.Next<float>(0, groundSize.x), rnd.Next<float>(0, groundSize.y) };
				f = gLooper.res.env_grass2_[rnd.Next<uint32_t>(ResTpFrames::_countof_env_grass2_)];
				s.x = rnd.Next<float>(scaleRange.from, scaleRange.to);
				s.y = rnd.Next<float>(scaleRange.from, scaleRange.to);
				grasses.Emplace().Emplace<EnvGrass>()->Init(stage, std::move(f), pos, s, swingRange);
			}
		}

		std::sort(grasses.buf, grasses.buf + grasses.len
			, [](auto& a, auto& b) { return a->pos.y < b->pos.y; });
	}

	inline void Ground2::Update() {
		for (auto i = grasses.len - 1; i >= 0; --i) {
			auto& o = grasses[i];
			if (o->Update()) {
				grasses.SwapRemoveAt(i);
			}
		}
	}

	inline void Ground2::Clear() {
		grasses.Clear();
	}

	inline void Ground2::Draw() {
		auto& camera = stage->camera;

		static constexpr XY tileSize{ 128, 128 };

		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstanceTiled).Draw(gLooper.res_map_01, tex);
		q->pos = camera.ToGLPos({ 0, stage->mapSize.y });
		q->scale = camera.scale;
		q->tileSize = tileSize;
		q->mapSize = stage->gridSize * tileSize;
		q->tiling = 1;
		q->offset = 0;
		q->miniMapSize = stage->gridSize;
	}

}
