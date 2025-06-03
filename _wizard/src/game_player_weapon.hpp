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

		if (gLooper.mouse.PressedMBLeft() && !gLooper.mouseEventHandler) {
			auto sPos = GetShootPos();
			stage->playerBullets.Emplace().Emplace<PlayerBullet>()->Init(this, sPos, radians);
			gLooper.sound.Play(gLooper.res_sound_laster_gun_1, 0.1f);
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
			.Draw(gLooper.res._texid_light_monster_bullet, 1);
		q->pos = stage->camera.ToGLPos(GetShootPos());
		q->anchor = 0.5f;
		q->scale = 32.f / gLooper.res._size_light_monster_bullet.y * stage->camera.scale * 3.f;
		q->radians = 0.f;
		q->colorplus = 1.f;
		q->color = xx::RGBA8_White;
		q->texRect.data = ResTpFrames::_uvrect_light_monster_bullet.data;
	}
}
