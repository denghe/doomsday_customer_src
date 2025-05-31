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
		lightColor = cLightColor;
		lightColorPlus = cLightColorPlus;
		lightRadius = ResTpFrames::_size_ef_light * 0.5f * cLightRadiusRatio;
		monsterFormation = std::move(monsterFormation_);
		monsterFormationPosIdx = monsterFormationPosIdx_;
	}

	XX_INLINE void Monster::FaceToPlayer() {
		static constexpr auto playerSize_2 = Player::cSize * 0.5f;	// x: radius
		//auto playerPos = stage->player->GetPosLT() + playerSize_2;	// center pos
		auto playerPos = stage->player->pos;
		auto d = XY{ playerPos } - pos;
		radians = std::atan2f(d.y, d.x);
	}

	XX_INLINE void Monster::TryShoot() {
		// calc shoot pos
		auto cos = std::cosf(radians);
		auto sin = std::sinf(radians);
		auto s = radius * 2.f / frame->spriteSize.x;
		auto x = frame->spriteSize.x * cShootOffset.x * s;
		auto y = frame->spriteSize.y * cShootOffset.y * s;
		auto sPos = pos + XY{ x * cos - y * sin, x * sin + y * cos };

		// shoot logic
		shootTimePool += Cfg::frameDelay;
		while (shootTimePool >= cShootDelay) {
			shootTimePool -= cShootDelay;
			auto b = xx::MakeShared<MonsterBullet>();
			b->Init(this, sPos, radians);
			stage->monsterBullets.Add(std::move(b));
		}
	}

	XX_INLINE bool Monster::FlyToTarget() {
		auto tarPos = monsterFormation->GetPos(monsterFormationPosIdx);
		auto d = tarPos - pos;
		auto dd = d.x * d.x + d.y * d.y;
		if (dd < cMoveSpeed * cMoveSpeed) {
			pos = tarPos;
			return true;
		}
		else {
			auto mag = std::sqrtf(dd);
			auto norm = d / mag;
			pos += norm * cMoveSpeed;
			return false;
		}
	}

	XX_INLINE void Monster::HandleBlock() {
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
	}

	XX_INLINE void Monster::SyncPos() {
		stage->ForceLimit(pos);
		stage->monsters.Update(this);	// sync space index
	}

	inline int32_t Monster::Update() {
		XX_BEGIN(_n);
	LabFly:
		XX_YIELD_I(_n);
		{
		bool success = FlyToTarget();
		FaceToPlayer();
		SyncPos();
		if (!success) goto LabFly;
		}
	LabFight:
		XX_YIELD_I(_n);
		++stage->numReadyMonsters;
		FlyToTarget();
		HandleBlock();
		FaceToPlayer();
		SyncPos();
		TryShoot();
		goto LabFight;
		XX_END(_n);
		return 0;
	}

	inline void Monster::Draw() {
		auto& f = *frame;
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(f.tex, 1);
		q->pos = stage->camera.ToGLPos(pos);
		q->anchor = *f.anchor;
		q->scale = radius * 2.f / f.spriteSize.x * stage->camera.scale;
		q->radians = radians;
		q->colorplus = 1.f;
		q->color = xx::RGBA8_White;
		q->texRect.data = f.textureRect.data;
	}

}
