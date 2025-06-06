#pragma once

namespace Game {

	inline void MonsterBullet::Init(Monster* shooter_, XY pos_, float radians_) {
		assert(!stage);
		stage = shooter_->stage;
		pos = pos_;
		radians = radians_;
		radius = 6.f;
		moveInc = { std::cosf(radians) * cMoveSpeed, std::sinf(radians) * cMoveSpeed };
		mp2 = shooter_->mp;
	}

	inline int32_t MonsterBullet::Update() {

		{
			static constexpr auto playerSize_2 = Player::cSize * 0.5f;	// x: radius
			auto playerPos = stage->player->GetPosLT() + playerSize_2;	// center pos
			auto d = playerPos - pos;
			auto dd = d.x * d.x + d.y * d.y;
			auto r = radius + playerSize_2.x;
			auto rr = r * r;
			if (dd < rr) {
				stage->player->Hurt(mp2.damagePoint);
				PlayDeathEffect(0.5f);
				return 1;
			}
		}

		pos += moveInc;

		// out of map check
		auto& gs = stage->map->blocks.gridSize;
		if (pos.x < 0 || pos.y < 0 || pos.x >= gs.x || pos.y >= gs.y)
			return 1;

		// handle blocks
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
					if (auto bc = blocks.TryAt({ colIdx, rowIdx }); bc) {
						if (bc->IsCross(iPosLT, size)) {
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
		if (frameIndex >= gLooper.res._countof_dark_bullet_) {
			frameIndex = 0;
		}

		return 0;
	}

	inline void MonsterBullet::Draw() {
		auto& frame = gLooper.res.dark_bullet_[(int32_t)frameIndex];
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(frame->tex, 1);
		q->pos = stage->camera.ToGLPos(pos);
		q->anchor = *frame->anchor;
		q->scale = radius * 2.f / frame->spriteSize.y * stage->camera.scale;
		q->radians = radians;
		q->colorplus = 1.f;
		q->color = xx::RGBA8_White;
		q->texRect.data = frame->textureRect.data;
	}

	inline void MonsterBullet::DrawLight() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res._texid_ef_light64, 1);
		q->pos = stage->camera.ToGLPos(pos);
		q->anchor = 0.5f;
		q->scale = radius / (gLooper.res._size_dark_bullet_0.y * 0.5f) * stage->camera.scale * 5.f;
		q->radians = 0.f;
		q->colorplus = 1.f;
		q->color = { 0x35,0,0xcb,0xff };
		q->texRect.data = ResTpFrames::_uvrect_ef_light64.data;
	}

	XX_INLINE void MonsterBullet::PlayDeathEffect(float scale_) {
		gLooper.sound.Play(gLooper.res_sound_hit_1, scale_);
		stage->effectExplosions.Emplace().Init(pos, scale_, cLightColor);
	}

	XX_INLINE std::pair<float, int> MonsterBullet::Hurt(float dp) {
		// todo
		PlayDeathEffect(0.5f);
		return { dp, 2 };
	}

}
