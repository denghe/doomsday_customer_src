#pragma once

namespace Game {

	inline void PlayerWeapon::Init(Player* owner_, XY holdOffset_) {
		assert(!stage);
		stage = owner_->stage;
		owner = xx::WeakFromThis(owner_);
		holdOffset = holdOffset_;
		pos = owner->pos + holdOffset;
		pwp.Init();
		pwp.CalcAll();
	}

	inline int32_t PlayerWeapon::Update() {
		if (!owner) return 1;
		pos = owner->pos + holdOffset;
		auto mp = stage->camera.ToLogicPos(gLooper.mouse.pos);
		auto d = mp - pos;
		radians = std::atan2f(d.y, d.x);
		auto sPos = GetShootPos();

		// todo: projectileAmount spread
		if (stage->time >= nextShootTime && !gLooper.mouseEventHandler) {
			if (gLooper.mouse.PressedMBRight()) {
				if (owner->pp.mana >= pwp.manaCost) {
					owner->pp.mana -= pwp.manaCost;
					nextShootTime = stage->time + int32_t(Cfg::fps / pwp.shootSpeed);
					for (int32_t i = 0; i < (int32_t)pwp.projectileAmount; ++i) {
						auto v = float(M_PI) / 180.f * pwp.spread;
						auto r = stage->rnd.Next<float>(-v, v);
						stage->playerBullets.Emplace().Emplace<PlayerBullet_FireB>()->Init(this, sPos, radians + r);
					}
				}
			}
			else if (gLooper.mouse.PressedMBLeft()) {
				nextShootTime = stage->time + int32_t(Cfg::fps / pwp.shootSpeed);
				for (int32_t i = 0; i < (int32_t)pwp.projectileAmount; ++i) {
					auto v = float(M_PI) / 180.f * pwp.spread;
					auto r = stage->rnd.Next<float>(-v, v);
					stage->playerBullets.Emplace().Emplace<PlayerBullet_FireA>()->Init(this, sPos, radians + r);
				}
			}
		}
		return 0;
	}

	XX_INLINE XY PlayerWeapon::GetShootPos() {
		auto cos = std::cosf(radians);
		auto sin = std::sinf(radians);
		return pos + XY{ cShootOffset.x * cos - cShootOffset.y * sin, cShootOffset.x * sin + cShootOffset.y * cos };
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

	inline void PlayerWeapon::DrawLight() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res._texid_ef_light64, 1);
		q->pos = stage->camera.ToGLPos(GetShootPos());
		q->anchor = 0.5f;
		q->scale = stage->camera.scale * 2.f;
		q->radians = 0.f;
		q->colorplus = 1.f;
		q->color = xx::RGBA8_White;
		q->texRect.data = ResTpFrames::_uvrect_ef_light64.data;
	}
}
