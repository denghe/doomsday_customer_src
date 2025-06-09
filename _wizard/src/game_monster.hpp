#pragma once

namespace Game {

	inline void Monster::Init(Stage* stage_, XY bornPos, int32_t flyTargetsIdx_) {
		assert(!stage);
		stage = stage_;
		frame = gLooper.res.monster_1;
		lightFrame = gLooper.res.light_monster_1;
		pos = bornPos;
		radians = 0;
		radius = 50.f / 2;
		flyTargetsIdx = flyTargetsIdx_;
		mp.Init();
		shaker.Shake(20, 500.f * Cfg::frameDelay, 999999999, stage->time);
	}

	XX_INLINE void Monster::TracePlayer() {
		auto playerPos = stage->player->centerPos;
		auto d = playerPos - pos;
		radians = std::atan2f(d.y, d.x);
		auto cos = std::cosf(radians);
		auto sin = std::sinf(radians);
		auto s = radius * 2.f / frame->spriteSize.x;
		auto x = frame->spriteSize.x * cShootOffset.x * s;
		auto y = frame->spriteSize.y * cShootOffset.y * s;
		shootPos = pos + XY{ x * cos - y * sin, x * sin + y * cos };

		if (stage->time < rayTracePlayerTime) return;
		rayTracePlayerTime = stage->time + cRayTracePlayerInterval;

		d = playerPos - shootPos;
		auto distance = std::sqrt(d.x * d.x + d.y * d.y);	// todo: max distance check ?
		auto boxRadius = stage->player->radius * 0.8f;
		if (distance > boxRadius + stage->player->radius) {
			auto& blocks = stage->map->blocks;
			auto numSteps = (int32_t)(distance / boxRadius);
			auto stepDistance = distance / (float)numSteps;
			XY stepInc{ cos * stepDistance, sin * stepDistance };
			auto topLeftPos = shootPos - boxRadius;
			auto rightBottomPos = shootPos + boxRadius;
			for (int32_t i = 0; i < numSteps - 1; ++i) {
				topLeftPos += stepInc;
				rightBottomPos += stepInc;
				auto criFrom = blocks.PosToColRowIndex(topLeftPos);
				auto criTo = blocks.PosToColRowIndex(rightBottomPos);
				for (int rowIdx = criFrom.y; rowIdx <= criTo.y; ++rowIdx) {
					for (int colIdx = criFrom.x; colIdx <= criTo.x; ++colIdx) {
						if (auto bc = blocks.At({ colIdx, rowIdx }); bc) {
							if (bc->IsCrossBox(topLeftPos, boxRadius * 2.f)) {
								rayTracePlayerSuccess = false;
								return;
							}
						}
					}
				}
			}
		}
		rayTracePlayerSuccess = true;
	}

	XX_INLINE void Monster::TryShoot() {
		if (stage->time < shootTime) return;
		shootTime = stage->time + cShootDelay;
		auto b = xx::MakeShared<MonsterBullet>();
		b->Init(this, shootPos, radians);
		stage->monsterBullets.Add(std::move(b));
	}


	template<bool withOffset>
	XX_INLINE XY Monster::GetFlyTargetPos() {
		if constexpr (withOffset) {
			return stage->map->GetFlyTargetPosWithOffset(flyTargetsIdx);
		}
		else {
			return stage->map->GetFlyTargetPos(flyTargetsIdx);
		}
	}

	XX_INLINE bool Monster::MoveTo(XY tarPos, float speed) {
		auto d = tarPos - pos;
		auto dd = d.x * d.x + d.y * d.y;
		if (dd < speed * speed) {
			pos = tarPos;
			return true;
		}
		else {
			auto mag = std::sqrtf(dd);
			auto norm = d / mag;
			pos += norm * speed;
			return false;
		}
	}

	XX_INLINE void Monster::MoveToPlayer(float speed) {
		auto s = stage->map->GetFlowFieldVec(pos, radius, stage->player->centerPos);
		pos += s * speed;
	}

	XX_INLINE void Monster::HandleBlocks() {
		auto& blocks = stage->map->blocks;
		auto criFrom = blocks.PosToColRowIndex(pos - radius);
		auto criTo = blocks.PosToColRowIndex(pos + radius - 0.01f);
		// check cross & pushout
		for (int rowIdx = criFrom.y; rowIdx <= criTo.y; ++rowIdx) {
			for (int colIdx = criFrom.x; colIdx <= criTo.x; ++colIdx) {
				if (auto bc = blocks.At({ colIdx, rowIdx }); bc) {
					bc->PushOutCircle(pos, radius);
				}
			}
		}
		stage->ForceLimit(pos);
	}

	XX_INLINE void Monster::SyncPos() {
		stage->ForceLimit(pos);
		stage->monsters.Update(this);	// sync space index
	}

	inline int32_t Monster::Update() {
		if (stunEndTime >= stage->time) return 0;	// when stun: pause all action
		XX_BEGIN(_n);
	LabFlyToFormationPos:
		XX_YIELD_I(_n);
		{
		bool isFinish = MoveTo(GetFlyTargetPos<false>(), cFlySpeed);
		TracePlayer();
		SyncPos();
		if (!isFinish) goto LabFlyToFormationPos;
		}
	LabFormationFight:
		XX_YIELD_I(_n);
		++stage->numReadyMonsters;
		if (rayTracePlayerSuccess) {
			shaker.Update(stage->rnd, stage->time);
			MoveTo(GetFlyTargetPos<true>() + shaker.offset, cMoveSpeed);
		}
		else {
			MoveTo(GetFlyTargetPos<true>(), cMoveSpeed);
		}
		HandleBlocks();
		TracePlayer();
		SyncPos();
		if (rayTracePlayerSuccess) {
			TryShoot();
			goto LabFormationFight;
		}
	LabFight:
		XX_YIELD_I(_n);
		++stage->numReadyMonsters;
		if (rayTracePlayerSuccess) {
			shaker.Update(stage->rnd, stage->time);
			MoveTo(pos + shaker.offset, cTraceSpeed);
		}
		else {
			MoveToPlayer(cTraceSpeed);
		}
		HandleBlocks();
		TracePlayer();
		SyncPos();
		if (rayTracePlayerSuccess) {
			TryShoot();
		}
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
		q->colorplus = stunEndTime >= stage->time ? 100000.f : 1.f;
		q->color = xx::RGBA8_White;
		q->texRect.data = f.textureRect.data;
	}

	inline void Monster::DrawLight() {
		xx::Frame* f;
		float cp;
		if (stunEndTime >= stage->time) {
			f = frame;
			cp = 100000.f;
		}
		else {
			f = lightFrame;
			cp = 1.f;
		}
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(f->tex, 1);
		q->pos = stage->camera.ToGLPos(pos);
		q->anchor = *f->anchor;
		q->scale = radius * 2.f / f->spriteSize.x * stage->camera.scale;
		q->radians = radians;
		q->colorplus = cp;
		q->color = xx::RGBA8_White;
		q->texRect.data = f->textureRect.data;
	}

	XX_INLINE void Monster::PlayDeathEffect(float scale_) {
		gLooper.sound.Play(gLooper.res_sound_bomb_1, scale_);
		stage->camera.shaker.Shake(5, 300.f * Cfg::frameDelay, int32_t(0.2f * scale_ * Cfg::fps), stage->time);
		stage->effectExplosions.Emplace().Init(pos, 3.f * scale_, { 0x77,22,22,0xff });
	}

	XX_INLINE std::pair<float, int> Monster::Hurt(float dp) {
		auto r = mp.Hurt(dp);
		if (r.second == 2) {	// dead
			PlayDeathEffect(1.f);
		}
		else {	// not dead
			stunEndTime = stage->time + int32_t(0.1f * Cfg::fps);
			// todo: draw hp bar?
			// change to white?
			// stun?
			// knockback?
			// dot? buff?
		}
		return r;
	}

}
