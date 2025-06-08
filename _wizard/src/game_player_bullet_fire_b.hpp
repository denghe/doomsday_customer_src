#pragma once

namespace Game {

	inline void PlayerBullet_FireB::Init(PlayerWeapon* shooter, XY pos_, float radians_) {
		assert(!stage);
		owner = shooter->owner;
		pwp2b = shooter->pwp;
		stage = owner->stage;
		pos = pos_;
		radians = radians_;
		radius = 16.f * pwp2b.scale;
		auto spd = pwp2b.movementSpeed * Cfg::frameDelay;
		moveInc = { std::cosf(radians) * spd, std::sinf(radians) * spd };
		gLooper.sound.Play(gLooper.res_sound_shoot_2, 0.2f);
	}

	inline int32_t PlayerBullet_FireB::Update() {
		assert(owner);
		if (HitCheck()) return 1;
		if (Move()) return 1;
		return 0;
	}

	inline void PlayerBullet_FireB::Draw() {
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

	inline void PlayerBullet_FireB::DrawLight() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res._texid_ef_light64, 1);
		q->pos = stage->camera.ToGLPos(pos);
		q->anchor = 0.5f;
		q->scale = radius / (gLooper.res._size_fire_bullet_0.y * 0.5f) * stage->camera.scale * 5.f;
		q->radians = 0.f;
		q->colorplus = 0.7f;
		q->color = cLightColor;
		q->texRect.data = ResTpFrames::_uvrect_ef_light64.data;
	}

	XX_INLINE bool PlayerBullet_FireB::Move() {
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

	XX_INLINE bool PlayerBullet_FireB::HitCheck() {

		// hit check ( monster bullet )
		if (stage->monsterBullets.FindFirstCrossBy9(pos.x, pos.y, radius)) {
			goto LabExpbloding;
		}
		// hit check ( monster )
		if (auto o = stage->monsters.FindFirstCrossBy9(pos.x, pos.y, radius)) {
			goto LabExpbloding;
		}
		// hit check ( blocks )
		{
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
					if (auto bc = blocks.At({ colIdx, rowIdx }); bc) {
						if (bc->IsCrossBox(iPosLT, size)) {
							goto LabExpbloding;
						}
					}
				}
			}
		}

		return false;

	LabExpbloding:
		radius *= cExplodeRadiusRatio;

		auto dp = owner->pp.CalcDamagePoint(stage->rnd, pwp2b.damagePoint);

		stage->monsterBullets.ForeachByRange(gLooper.rdd, pos, int32_t(radius + Cfg::unitSize), [this, dp](MonsterBullet* o) {
			auto d = o->pos - pos;
			auto r = radius + o->radius;
			if (d.x * d.x + d.y * d.y < r * r) {
				auto [v, dead] = o->Hurt(dp.first);
				if(dead) stage->monsterBullets.Remove(o);
			}
		});

		stage->monsters.ForeachByRange(gLooper.rdd, pos, int32_t(radius + Cfg::unitSize), [this, dp](Monster* o) {
			auto d = o->pos - pos;
			auto r = radius + o->radius;
			if (d.x * d.x + d.y * d.y < r * r) {
				auto [v, dead] = o->Hurt(dp.first);
				stage->effectTexts.Add(o->pos, o->pos - pos, dp.second ? xx::RGBA8_Red : xx::RGBA8_White, 3.f, (int32_t)v);
				if(dead) stage->monsters.Remove(o);
			}
		});

		// play effect
		gLooper.sound.Play(gLooper.res_sound_bomb_2);
		auto r = radius * 2.f / gLooper.res._size_fire_explosion_0.x;
		stage->effectExplosions.Emplace().Init(&gLooper.res.fire_explosion_, 15, pos, r, cLightColor);

		return true;
	}

}
