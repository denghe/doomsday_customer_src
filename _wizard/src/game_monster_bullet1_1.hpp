#pragma once

namespace Game {

	inline void MonsterBullet1_1::Init(Monster* shooter_, XY pos_, float radians_) {
		assert(!stage);
		stage = shooter_->stage;
		pos = pos_;
		radians = radians_;
		radius = 16.f;
		moveInc = { std::cosf(radians) * cMoveSpeed, std::sinf(radians) * cMoveSpeed };
		mp2 = shooter_->mp;
	}

	inline int32_t MonsterBullet1_1::Update() {

		// hit player check
		{
			auto playerPos = stage->player->centerPos;
			auto d = playerPos - pos;
			auto dd = d.x * d.x + d.y * d.y;
			auto r = radius + stage->player->radius;
			auto rr = r * r;
			if (dd < rr) {
				stage->player->Hurt(mp2.damagePoint);
				PlayDeathEffect(0.5f);
				return 1;
			}
		}

		pos += moveInc;


		// handle blocks
		{
			auto& gs = stage->map->blocks.gridSize;
			auto iPos = pos.As<int32_t>();
			auto r = radius * 0.5f;
			auto iPosLT = iPos - int32_t(r);
			auto iPosRB = iPos + int32_t(r);
			if (iPosLT.x < 0 || iPosLT.y < 0 || iPosRB.x >= gs.x || iPosRB.y >= gs.y) return 1;
			XYi size{ r * 2.f };

			auto& blocks = stage->map->blocks;
			auto criFrom = blocks.PosToColRowIndex(iPosLT);
			auto criTo = blocks.PosToColRowIndex(iPosRB - 1);
			for (int rowIdx = criFrom.y; rowIdx <= criTo.y; ++rowIdx) {
				for (int colIdx = criFrom.x; colIdx <= criTo.x; ++colIdx) {
					if (auto bc = blocks.At({ colIdx, rowIdx }); bc) {
						if (bc->IsCrossBox(iPosLT, size)) {
							PlayDeathEffect(1.f);
							return 1;
						}
					}
				}
			}
		}

		stage->monsterBullets.Update(this);	// sync space index

		// frame step
		frameIndex += 15.f / Cfg::fps;
		if (frameIndex >= gLooper.res._countof_bullet1_) {
			frameIndex = 0;
		}

		return 0;
	}

	inline void MonsterBullet1_1::Draw() {
		auto& frame = gLooper.res.bullet1_[(int32_t)frameIndex];
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(frame->tex, 1);
		q->pos = stage->camera.ToGLPos(pos);
		q->anchor = *frame->anchor;
		q->scale = radius * 2.f / frame->spriteSize.y * stage->camera.scale;
		q->radians = radians;
		q->colorplus = 1.f;
		q->color = { 0xF9,0x4b,0xc3,0xff };
		//q->color = xx::RGBA8_White;
		q->texRect.data = frame->textureRect.data;
	}

	inline void MonsterBullet1_1::DrawLight(float colorPlus_) {
		auto& frame = gLooper.res.light_bullet1_[(int32_t)frameIndex];
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(frame->tex, 1);
		q->pos = stage->camera.ToGLPos(pos);
		q->anchor = *frame->anchor;
		q->scale = radius * 2.f / frame->spriteSize.y * stage->camera.scale * 3.f;
		q->radians = radians;
		q->colorplus = colorPlus_;
		q->color = { 0xF9,0x4b,0xc3,0xff };
		//q->color = xx::RGBA8_White;
		q->texRect.data = frame->textureRect.data;
	}

	XX_INLINE void MonsterBullet1_1::PlayDeathEffect(float scale_) {
		gLooper.sound.Play(gLooper.res_sound_hit_1, scale_);
		stage->effectExplosions.Emplace().Init(pos, scale_, cLightColor);
	}

}
