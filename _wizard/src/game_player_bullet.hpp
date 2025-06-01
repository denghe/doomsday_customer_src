#pragma once

namespace Game {

	inline void PlayerBullet::Init(PlayerWeapon* shooter, XY pos_, float radians_) {
		assert(!stage);
		owner = shooter->owner;
		stage = owner->stage;
		pos = pos_;
		radians = radians_;
		radius = 12.f;
		moveInc = { std::cosf(radians) * cMoveSpeed, std::sinf(radians) * cMoveSpeed };
		pierceCount = 1;
		cPierceDelay = int32_t(0.1f * Cfg::fps);
	}

	inline int32_t PlayerBullet::Update() {
		assert(owner);

		// pierce: clear timeout data
		auto now = stage->time;
		auto newTime = now + cPierceDelay;
		for (int32_t i = pierceBlackList.len - 1; i >= 0; --i) {
			if (pierceBlackList[i].second < now) {
				pierceBlackList.SwapRemoveAt(i);
			}
		}

		// hit check ( monster bullet )
		stage->monsterBullets.Foreach9All(pos.As<int32_t>(), [this, newTime](MonsterBullet* o)->bool {
			auto d = o->pos - pos;
			auto r = radius + o->radius;
			if (d.x * d.x + d.y * d.y < r * r) {
				for (int32_t i = 0, e = pierceBlackList.len; i < e; ++i) {	// check black list
					if (pierceBlackList[i].first.GetPointer() == o) return false;     // exists: continue
				}
				// todo: o.Hurt(this);
				if (true) {	// o is dead
					gLooper.sound.Play(gLooper.res_sound_hit_1);
					stage->effectExplosions.Emplace().Init(o->pos, 0.5f, { 0x35,0,0xcb,0xff });
					stage->monsterBullets.Remove(o);
				}
				else {
					pierceBlackList.Emplace(xx::WeakFromThis(o), newTime);	// add to black list
				}
				if (--pierceCount < 0) return true;
			}
			return false;
		});
		if (pierceCount < 0) {
			gLooper.sound.Play(gLooper.res_sound_hit_1);
			stage->effectExplosions.Emplace().Init(pos, 0.5f);
			return 1;
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
			//auto d = pos - o->pos;
			//auto dmg = damage * damageRatio;
			//bool isCrit{};
			//if (stage->rnd.Next<float>(0, 1) < criticalChance) {
			//	dmg *= criticalBonusRatio;
			//	isCrit = true;
			//}
			//auto dead = o->Hurt(dmg, d, -d, isCrit);
			gLooper.sound.Play(gLooper.res_sound_monster_die_1);
			stage->camera.Shake(5, 300.f * Cfg::frameDelay, int32_t(0.2f * Cfg::fps), stage->time);

			stage->effectExplosions.Emplace().Init(o->pos, 3.f, { 0x77,22,22,0xff });
			stage->monsters.Remove(o);

			stage->effectExplosions.Emplace().Init(pos, 0.5f);
			return 1;
		}


		pos += moveInc;

		// out of map check
		auto& gs = stage->map->blocks.gridSize;
		if (pos.x < 0 || pos.y < 0 || pos.x >= gs.x || pos.y >= gs.y)
			return 1;

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
						gLooper.sound.Play(gLooper.res_sound_hit_1);
						stage->effectExplosions.Emplace().Init(pos, 1.f);
						return 1;
					}
				}
			}
		}


		return 0;
	}

	inline void PlayerBullet::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res._texid_char_bullet, 1);
		q->pos = stage->camera.ToGLPos(pos);
		q->anchor = gLooper.res._anchor_char_bullet;
		q->scale = radius / gLooper.res._size_char_bullet.y * stage->camera.scale;
		q->radians = radians;
		q->colorplus = 0.5f;
		q->color = xx::RGBA8_White;
		q->texRect.data = ResTpFrames::_uvrect_char_bullet.data;
	}

	inline void PlayerBullet::DrawLight() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res._texid_light_monster_bullet, 1);
		q->pos = stage->camera.ToGLPos(pos);
		q->anchor = 0.5f;
		q->scale = radius / gLooper.res._size_char_bullet.y * 32.f / gLooper.res._size_light_monster_bullet.y * stage->camera.scale * 5.f;
		q->radians = 0.f;
		q->colorplus = 0.5f;
		q->color = xx::RGBA8_White;
		q->texRect.data = ResTpFrames::_uvrect_light_monster_bullet.data;
	}

}
