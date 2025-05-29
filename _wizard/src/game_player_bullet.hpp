#pragma once

namespace Game {

	inline void PlayerBullet::Init(PlayerWeapon* shooter, XY pos_, float radians_) {
		owner = shooter->owner;
		stage = owner->stage;
		pos = pos_;
		radians = radians_;
		radius = 16.f;
		lightRadius = ResTpFrames::_size_char_bullet * 0.5f * 3.f;
		moveInc = { std::cosf(radians) * cMoveSpeed, std::sinf(radians) * cMoveSpeed };
	}

	inline int32_t PlayerBullet::Update() {
		if (!owner) return 1;
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
						stage->effectExplosions.Emplace().Init(&gLooper.res.ef_explosion_, 30.f, pos, 1.f);
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
		q->pos = stage->camera.ToGLPos(pos) * stage->scale;
		q->anchor = gLooper.res._anchor_char_bullet;
		q->scale = radius / gLooper.res._size_char_bullet.y * stage->camera.scale * stage->scale;
		q->radians = radians;
		q->colorplus = 1.f;
		q->color = xx::RGBA8_White;
		q->texRect.data = ResTpFrames::_uvrect_char_bullet.data;
	}
}
