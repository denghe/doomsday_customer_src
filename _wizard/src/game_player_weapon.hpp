#pragma once

namespace Game {

	inline void PlayerWeapon::Init(Player* owner_, XY holdOffset_) {
		assert(!stage);
		stage = owner_->stage;
		owner = xx::WeakFromThis(owner_);
		holdOffset = holdOffset_;
		pwp.Init();
		pwp.CalcAll();
		UpdatePosRadians();
	}

	XX_INLINE void PlayerWeapon::UpdatePosRadians() {
		pos = owner->pos + holdOffset;
		auto mp = stage->camera.ToLogicPos(gLooper.mouse.pos);
		auto d = mp - pos;
		radians = std::atan2f(d.y, d.x);
		cos = std::cosf(radians);
		sin = std::sinf(radians);
		shootPos = pos + XY{ cShootOffset.x * cos - cShootOffset.y * sin
			, cShootOffset.x * sin + cShootOffset.y * cos };
	}

	inline int32_t PlayerWeapon::Update() {
		if (!owner) return 1;
		UpdatePosRadians();

		if (stage->time >= nextShootTime && !gLooper.mouseEventHandler) {
			if (gLooper.mouse.PressedMBRight()) {
				if (owner->pp.mana >= pwp.manaCost) {
					owner->pp.mana -= pwp.manaCost;
					nextShootTime = stage->time + int32_t(Cfg::fps / pwp.shootSpeed);
					for (int32_t i = 0; i < (int32_t)pwp.projectileAmount; ++i) {
						auto v = float(M_PI) / 180.f * pwp.spread;
						auto r = stage->rnd.Next<float>(-v, v);
						stage->playerBullets.Emplace().Emplace<PlayerBullet_FireB>()->Init(this, shootPos, radians + r);
					}
				}
			}
			else if (gLooper.mouse.PressedMBLeft()) {
				nextShootTime = stage->time + int32_t(Cfg::fps / pwp.shootSpeed * 0.5f);
				for (int32_t i = 0; i < (int32_t)pwp.projectileAmount; ++i) {
					auto v = float(M_PI) / 180.f * pwp.spread;
					auto r = stage->rnd.Next<float>(-v, v);
					stage->playerBullets.Emplace().Emplace<PlayerBullet_FireA>()->Init(this, shootPos, radians + r);
				}
			}
		}
		return 0;
	}

	inline void PlayerWeapon::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res._texid_char_weapon, 1);
		q->pos = stage->camera.ToGLPos(pos);
		q->anchor = { cAnchorX, 0.5f };
		q->scale = stage->camera.scale;
		q->radians = radians;
		q->colorplus = 1.f;
		q->color = xx::RGBA8_White;
		q->texRect.data = ResTpFrames::_uvrect_char_weapon.data;
	}

	inline void PlayerWeapon::DrawLight(float colorPlus_) {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res._texid_ef_light64, 1);
		q->pos = stage->camera.ToGLPos(shootPos);
		q->anchor = 0.5f;
		q->scale = stage->camera.scale * 2.f;
		q->radians = 0.f;
		q->colorplus = colorPlus_;
		q->color = xx::RGBA8_White;
		q->texRect.data = ResTpFrames::_uvrect_ef_light64.data;
	}

	/********************************************************************************/
	/********************************************************************************/

	inline int32_t PlayerWeapon_1::Update() {
		if (!owner) return 1;
		UpdatePosRadians();

		if (!gLooper.mouseEventHandler) {
			if (gLooper.mouse.PressedMBRight()) {
				if (owner->pp.mana >= pwp.manaCost * Cfg::frameDelay) {
					owner->pp.mana -= pwp.manaCost * Cfg::frameDelay;
					shooting = 2;
					goto LabShoot;
				}
			}
			else if (gLooper.mouse.PressedMBLeft()) {
				shooting = 1;
				goto LabShoot;
			}
		}
		shooting = 0;
		return 0;

	LabShoot:
		uvOffset -= Cfg::frameDelay * 800;
		distance = cMaxDistance;

		// todo: if shooting == 1  == 2, switch  damage, frame, lightColor
		// shooting == 2: can pierce monster bullet & monster

		auto dp = owner->pp.CalcDamagePoint(stage->rnd, pwp.damagePoint);

		// clear timeout data
		auto now = stage->time;
		auto newTime = now + int32_t(pwp.hurtDelaySeconds * Cfg::fps);
		for (int32_t i = hitBlackList.len - 1; i >= 0; --i) {
			if (hitBlackList[i].second < now) {
				hitBlackList.SwapRemoveAt(i);
			}
		}

		// ray trace
		auto& blocks = stage->map->blocks;
		auto numSteps = (int32_t)(distance / cRayTraceBoxRadius);
		auto stepDistance = distance / (float)numSteps;
		XY stepInc{ cos * stepDistance, sin * stepDistance };
		auto centerPos = shootPos;
		auto topLeftPos = shootPos - cRayTraceBoxRadius;
		auto rightBottomPos = shootPos + cRayTraceBoxRadius;
		for (int32_t i = 0; i < numSteps - 1; ++i) {
			topLeftPos += stepInc;
			rightBottomPos += stepInc;
			if (topLeftPos.x < 0 || topLeftPos.y < 0
				|| rightBottomPos.x >= stage->mapSize.x || rightBottomPos.y >= stage->mapSize.y) {
				distance = stepDistance * i;
				goto LabBreak;
			}
			centerPos += stepInc;

			// hit check ( monster bullet )
			if (auto o = stage->monsterBullets.FindFirstCrossBy9(centerPos.x, centerPos.y, cRayTraceBoxRadius)) {
				distance = stepDistance * i;
				for (int32_t i = 0, e = hitBlackList.len; i < e; ++i) {	// check black list
					if (hitBlackList[i].first.GetPointer() == o)
						goto LabBreak;
				}

				auto tarPos = shootPos + XY{ cos * distance, sin * distance };
				auto [d, dead] = o->Hurt(dp.first);
				// don't need show damage text
				PlayDeathEffect(tarPos, 0.5f);
				if (dead) {
					stage->monsterBullets.Remove(o);
				}
				else {
					hitBlackList.Emplace(xx::WeakFromThis(o), newTime);	// add to black list
				}
				goto LabBreak;
			}

			// hit check ( monster )
			if (auto o = stage->monsters.FindFirstCrossBy9(centerPos.x, centerPos.y, cRayTraceBoxRadius)) {
				distance = stepDistance * i;
				for (int32_t i = 0, e = hitBlackList.len; i < e; ++i) {	// check black list
					if (hitBlackList[i].first.GetPointer() == o)
						goto LabBreak;
				}

				auto tarPos = shootPos + XY{ cos * distance, sin * distance };
				auto [d, dead] = o->Hurt(dp.first);
				stage->effectTexts.Add(tarPos, tarPos - o->pos, dp.second ? xx::RGBA8_Red : xx::RGBA8_White, 2.f, (int32_t)d);
				PlayDeathEffect(tarPos, 0.5f);
				if (dead) {
					stage->monsters.Remove(o);
				}
				else {
					hitBlackList.Emplace(xx::WeakFromThis(o), newTime);	// add to black list
				}
				goto LabBreak;
			}

			// hit blocks check
			auto criFrom = blocks.PosToColRowIndex(topLeftPos);
			auto criTo = blocks.PosToColRowIndex(rightBottomPos);
			for (int rowIdx = criFrom.y; rowIdx <= criTo.y; ++rowIdx) {
				for (int colIdx = criFrom.x; colIdx <= criTo.x; ++colIdx) {
					if (auto bc = blocks.At({ colIdx, rowIdx }); bc) {
						if (bc->IsCrossBox(topLeftPos, cRayTraceBoxRadius * 2.f)) {
							distance = stepDistance * i;
							PlayDeathEffect(shootPos + XY{ cos * distance, sin * distance }, 1.f);
							goto LabBreak;
						}
					}
				}
			}
		}
	LabBreak:
		return 0;
	}

	inline void PlayerWeapon_1::Draw() {
		PlayerWeapon::Draw();
		if (!shooting) return;
		// draw laser bullet
		auto f = gLooper.res.laser1;
		auto scale = distance / f->spriteSize.x * stage->camera.scale;
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstanceTilingOffset).Draw(f->tex, 1);
		q->pos = stage->camera.ToGLPos(shootPos);
		q->anchor = { 0, 0.5f };
		q->scale = { scale, 1 };
		q->radians = radians;
		q->colorplus = 1.f;
		q->color = { 115,223,255,255 };
		q->texRect.data = f->textureRect.data;
		q->tiling = { scale, 1 };
		q->offset = { uvOffset, 0 };
	}

	inline void PlayerWeapon_1::DrawLight(float colorPlus_) {
		PlayerWeapon::DrawLight(colorPlus_);
		if (!shooting) return;
		// draw laser bullet light
		auto f = gLooper.res.light_laser32;
		auto scale = distance / f->spriteSize.x * stage->camera.scale;
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstanceTilingOffset).Draw(f->tex, 1);
		q->pos = stage->camera.ToGLPos(shootPos);
		q->anchor = { 0, 0.5f };
		q->scale = { scale, 1 };
		q->radians = radians;
		q->colorplus = colorPlus_;
		q->color = { 115,223,255,255 };
		q->texRect.data = f->textureRect.data;
		q->tiling = 1;
		q->offset = 0;
	}

	XX_INLINE void PlayerWeapon_1::PlayDeathEffect(XY pos_, float scale_) {
		gLooper.sound.Play(gLooper.res_sound_hit_1, scale_);
		stage->effectExplosions.Emplace().Init(pos_, scale_, xx::RGBA8_White);
	}

}
