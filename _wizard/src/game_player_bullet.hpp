#pragma once

namespace Game {

	inline void PlayerBullet::Init(PlayerWeapon* shooter, XY pos_, float radians_) {
		assert(!stage);
		owner = shooter->owner;
		stage = owner->stage;
		pos = pos_;
		radians = radians_;
		radius = 16.f;
		lightColor = xx::RGBA8_Yellow;
		lightRadius = ResTpFrames::_size_char_bullet * 0.5f * 3.f;
		moveInc = { std::cosf(radians) * cMoveSpeed, std::sinf(radians) * cMoveSpeed };
	}

	inline int32_t PlayerBullet::Update() {
		if (!owner) return 1;

		// todo: hit check
		if (auto m = stage->monsters.FindFirstCrossBy9(pos.x, pos.y, radius)) {
			//stage->effects.Emplace().Emplace<EffectDeath>()->Init(stage, gLooper.res.bullet_coin5, pos, scale.x);
			//auto d = pos - m->pos;
			//auto dmg = damage * damageRatio;
			//bool isCrit{};
			//if (stage->rnd.Next<float>(0, 1) < criticalChance) {
			//	dmg *= criticalBonusRatio;
			//	isCrit = true;
			//}
			//m->Hurt(dmg, d, -d, isCrit);
			gLooper.sound.Play(gLooper.res_sound_monster_die_1);
			stage->camera.Shake(5, 300.f * Cfg::frameDelay, int32_t(0.2f * Cfg::fps), stage->time);
			stage->effectExplosions.Emplace().Init(pos, 0.5f, xx::RGBA8_Yellow);
			stage->effectExplosions.Emplace().Init(m->pos, 3.f, xx::RGBA8_Red);
			stage->monsters.Remove(m);
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
						stage->effectExplosions.Emplace().Init(pos, 1.f, lightColor);
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
		q->colorplus = 1.f;
		q->color = xx::RGBA8_White;
		q->texRect.data = ResTpFrames::_uvrect_char_bullet.data;
	}
}
