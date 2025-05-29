#pragma once

namespace Game {

	inline void PlayerWeapon::Init(Player* owner_, XY holdOffset_) {
		stage = owner_->stage;
		owner = xx::WeakFromThis(owner_);
		holdOffset = holdOffset_;
	}

	inline int32_t PlayerWeapon::Update() {
		if (!owner) return 1;
		pos = owner->pos + holdOffset;
		auto mp = stage->camera.ToLogicPos(gLooper.mouse.pos, stage->scale);
		auto d = mp - pos;
		radians = std::atan2f(d.y, d.x);
		return 0;
	}

	inline XY PlayerWeapon::GetShootPos() {
		return pos + XY{ std::cosf(radians), std::sinf(radians) } * cShootOffset;
	}

	inline void PlayerWeapon::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res._texid_char_weapon, 1);
		q->pos = stage->camera.ToGLPos(pos) * stage->scale;
		q->anchor = { cAnchorX, 0.5f };
		q->scale = stage->camera.scale * stage->scale;
		q->radians = radians;
		q->colorplus = 1.f;
		q->color = xx::RGBA8_White;
		q->texRect.data = ResTpFrames::_uvrect_char_weapon.data;
	}
}
