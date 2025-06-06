#pragma once

namespace Game {

	inline void PlayerBullet_FireA::Init(PlayerWeapon* shooter, XY pos_, float radians_) {
		assert(!stage);
		owner = shooter->owner;
		pwp2b = shooter->pwp;
		stage = owner->stage;
		pos = pos_;
		radians = radians_;
		radius = 6.f * pwp2b.scale;
		auto spd = pwp2b.movementSpeed * Cfg::frameDelay;
		moveInc = { std::cosf(radians) * spd, std::sinf(radians) * spd };
		gLooper.sound.Play(gLooper.res_sound_shoot_1, 0.2f);
	}

	inline int32_t PlayerBullet_FireA::Update() {
		assert(owner);
		if (HitCheck()) return 1;
		if (Move()) return 1;
		return 0;
	}

	inline void PlayerBullet_FireA::Draw() {
		auto& frame = gLooper.res.fire_bullet_[(int32_t)frameIndex];
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(frame->tex, 1);
		q->pos = stage->camera.ToGLPos(pos);
		q->anchor = *frame->anchor;
		q->scale = radius * 2.f / frame->spriteSize.y * stage->camera.scale;
		q->radians = radians;
		q->colorplus = 1.f;
		q->color = xx::RGBA8_White;
		q->texRect.data = frame->textureRect.data;
	}

	inline void PlayerBullet_FireA::DrawLight() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res._texid_ef_light64, 1);
		q->pos = stage->camera.ToGLPos(pos);
		q->anchor = 0.5f;
		q->scale = radius / (gLooper.res._size_fire_bullet_0.y * 0.5f) * stage->camera.scale * 5.f;
		q->radians = 0.f;
		q->colorplus = 0.5f;
		q->color = cLightColor;
		q->texRect.data = ResTpFrames::_uvrect_ef_light64.data;
	}

	XX_INLINE bool PlayerBullet_FireA::Move() {
		pos += moveInc;

		// out of map check
		auto& gs = stage->map->blocks.gridSize;
		if (pos.x < 0 || pos.y < 0 || pos.x >= gs.x || pos.y >= gs.y)
			return true;


		// frame step
		frameIndex += 15.f / Cfg::fps;
		if (frameIndex >= gLooper.res._countof_dark_bullet_) {
			frameIndex = 0;
		}

		return false;
	}

	XX_INLINE bool PlayerBullet_FireA::HitCheck() {
		// pierce: clear timeout data
		auto now = stage->time;
		auto newTime = now + int32_t(pwp2b.hurtDelaySeconds * Cfg::fps);
		for (int32_t i = pierceBlackList.len - 1; i >= 0; --i) {
			if (pierceBlackList[i].second < now) {
				pierceBlackList.SwapRemoveAt(i);
			}
		}

		auto dp = owner->pp.CalcDamagePoint(stage->rnd, pwp2b.damagePoint);

		// hit check ( monster bullet )
		stage->monsterBullets.Foreach9All(pos.As<int32_t>(), [this, newTime, dp](MonsterBullet* o)->bool {
			auto d = o->pos - pos;
			auto r = radius + o->radius;
			if (d.x * d.x + d.y * d.y < r * r) {
				for (int32_t i = 0, e = pierceBlackList.len; i < e; ++i) {	// check black list
					if (pierceBlackList[i].first.GetPointer() == o) return false;     // exists: continue
				}
				auto [d, dead] = o->Hurt(dp.first);
				// don't need show damage text
				if (dead) {
					stage->monsterBullets.Remove(o);
				} else {
					pierceBlackList.Emplace(xx::WeakFromThis(o), newTime);	// add to black list
				}
				if (--pwp2b.pierceCount < 0) return true;
			}
			return false;
			});
		if (pwp2b.pierceCount < 0) {
			gLooper.sound.Play(gLooper.res_sound_hit_1);
			stage->effectExplosions.Emplace().Init(pos, 0.5f, cLightColor);
			return true;
		}

		/*
			if (cfg.radius <= Scene.cellSize) monstersSpaceContainer.Foreach9All(x, y, HitCheck);
			else monstersSpaceContainer.ForeachByRange(Scene.sd, x, y, cfg.radius, HitCheck);
		 if canot pierce {
			if (auto o = stage->monsterBullets.FindFirstCrossBy9(pos.x, pos.y, radius)) {
				gLooper.sound.Play(gLooper.res_sound_hit_1);

				stage->effectExplosions.Emplace().Init(o->pos, 0.5f, { 0x35,0,0xcb,0xff });
				stage->monsterBullets.Remove(o);

				stage->effectExplosions.Emplace().Init(pos, 0.5f);
				return 1;
			}
		}
		*/

		// hit check ( monster )
		if (auto o = stage->monsters.FindFirstCrossBy9(pos.x, pos.y, radius)) {
			auto [d, dead] = o->Hurt(dp.first);
			stage->effectTexts.Add(pos, pos - o->pos, dp.second ? xx::RGBA8_Red : xx::RGBA8_White, 2.f, (int32_t)d);
			PlayDeathEffect(0.5f);
			if (dead) {
				stage->monsters.Remove(o);
			}
			return true;
		}

		// hit check ( blocks )
		auto iPos = pos.As<int32_t>();
		auto r = radius * 0.5f;
		auto iPosLT = iPos - int32_t(r);
		auto iPosRB = iPos + int32_t(r);
		XYi size{ int32_t(r) << 1 };

		auto& blocks = stage->map->blocks;
		auto criFrom = blocks.PosToColRowIndex(iPosLT);
		auto criTo = blocks.PosToColRowIndex(iPosRB - 1);
		for (int rowIdx = criFrom.y; rowIdx <= criTo.y; ++rowIdx) {
			for (int colIdx = criFrom.x; colIdx <= criTo.x; ++colIdx) {
				if (auto bc = blocks.TryAt({ colIdx, rowIdx }); bc) {
					if (bc->IsCross(iPosLT, size)) {
						PlayDeathEffect(1.f);
						return true;
					}
				}
			}
		}

		return false;
	}

	XX_INLINE void PlayerBullet_FireA::PlayDeathEffect(float scale_) {
		gLooper.sound.Play(gLooper.res_sound_hit_1, scale_);
		stage->effectExplosions.Emplace().Init(pos, scale_, cLightColor);
	}
}
