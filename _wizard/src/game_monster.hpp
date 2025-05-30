#pragma once

namespace Game {

	inline void Monster::Init(Stage* stage_, xx::Ref<xx::Frame> frame_, int32_t bornPosIndex) {
		stage = stage_;
		frame = std::move(frame_);
		auto posIdx = stage->map->bornPlaces_Monster[bornPosIndex];
		pos = posIdx * Cfg::unitSize + (Cfg::unitSize / 2);
		radians = 0;
		radius = 64.f;
		lightRadius = ResTpFrames::_size_char_bullet * 0.5f * 3.f;
		moveInc.x = 0;
		moveInc.y = 100.f / Cfg::fps;
	}

	inline int32_t Monster::Update() {

		// todo: move & fight logic
		pos += moveInc;

		// prepare
		auto& blocks = stage->map->blocks;
		auto size = XYi{ frame->spriteSize };

		auto iPosRB = pos.As<int32_t>();
		iPosRB.x += size.x >> 1;
		auto iPosLT = iPosRB - size;

		// handle blocks
		PushOutWays pushOutWays{};
		auto criFrom = blocks.PosToColRowIndex(iPosLT);
		auto criTo = blocks.PosToColRowIndex(iPosRB - 1);
		// check cross & pushout
		for (int rowIdx = criFrom.y; rowIdx <= criTo.y; ++rowIdx) {
			for (int colIdx = criFrom.x; colIdx <= criTo.x; ++colIdx) {
				if (auto bc = blocks.TryAt({ colIdx, rowIdx }); bc) {
					if (bc->IsCross(iPosLT, size)) {
						auto [newPos, pushOutWay] = bc->PushOut(iPosLT, size);
						if (pushOutWay != PushOutWays::Unknown) {
							iPosLT = newPos;
							iPosRB = iPosLT + size;
							(uint32_t&)pushOutWays |= (uint32_t&)pushOutWay;
						}
					}
				}
			}
		}
		if ((uint32_t)pushOutWays) {
			pos.x = float(iPosLT.x + (size.x >> 1));
			pos.y = float(iPosLT.y + size.y);
		}

		// out of map check
		if (iPosLT.x < 0 || iPosLT.y < 0)
			return 1;
		if (iPosRB.x >= blocks.gridSize.x || iPosRB.y >= blocks.gridSize.y)
			return 1;

		return 0;
	}

	inline void Monster::Draw() {
		auto& f = *frame;
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(f.tex, 1);
		q->pos = stage->camera.ToGLPos(pos) * stage->scale;
		q->anchor = *f.anchor;
		q->scale = radius / f.spriteSize.x * stage->camera.scale * stage->scale;
		q->radians = radians;
		q->colorplus = 1.f;
		q->color = xx::RGBA8_White;
		q->texRect.data = f.textureRect.data;
	}
}
