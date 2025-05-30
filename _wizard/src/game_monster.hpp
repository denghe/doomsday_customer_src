#pragma once

namespace Game {

	inline void Monster::Init(Stage* stage_, xx::Ref<xx::Frame> frame_, int32_t bornPosIdx
		, xx::Weak<MonsterFormation> monsterFormation_, int32_t monsterFormationPosIdx_) {
		assert(!stage);
		stage = stage_;
		frame = std::move(frame_);
		auto posIdx = stage->map->bornPlaces_Monster[bornPosIdx];
		pos = posIdx * Cfg::unitSize + (Cfg::unitSize / 2);
		radians = 0;
		radius = 64.f / 2;
		lightColor = xx::RGBA8_Red;
		lightRadius = ResTpFrames::_size_char_bullet * 0.5f * 3.f;
		monsterFormation = std::move(monsterFormation_);
		monsterFormationPosIdx = monsterFormationPosIdx_;
	}

	inline int32_t Monster::Update() {
		// todo: move & fight logic

		assert(monsterFormation);
		auto tarPos = monsterFormation->GetPos(monsterFormationPosIdx);
		

		auto d = tarPos - pos;
		auto dd = d.x * d.x + d.y * d.y;
		if (dd < cMoveSpeed * cMoveSpeed) {
			pos = tarPos;
			stage->numReadyMonsters++;
		}
		else {
			auto mag = std::sqrtf(dd);
			auto norm = d / mag;
			pos += norm * cMoveSpeed;
			stage->ForceLimit(pos);
			stage->monsters.Update(this);	// sync space index
		}

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
			stage->ForceLimit(pos);
		}

		return 0;
	}

	inline void Monster::Draw() {
		auto& f = *frame;
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(f.tex, 1);
		q->pos = stage->camera.ToGLPos(pos) * stage->scale;
		q->anchor = *f.anchor;
		q->scale = radius * 2.f / f.spriteSize.x * stage->camera.scale * stage->scale;
		q->radians = radians;
		q->colorplus = 1.f;
		q->color = xx::RGBA8_White;
		q->texRect.data = f.textureRect.data;
	}

}
