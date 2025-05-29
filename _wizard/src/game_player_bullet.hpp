#pragma once

namespace Game {

	inline void PlayerBullet::Init(Player* owner_) {
		stage = owner_->stage;
		owner = xx::WeakFromThis(owner_);
	}

	inline int32_t PlayerBullet::Update() {
		if (!owner) return 1;
		pos = owner->pos + XY{ 0, -32 };		// weapon pivot pos
		auto mp = stage->camera.ToLogicPos(gLooper.mouse.pos, stage->scale);
		auto d = mp - pos;
		radians = std::atan2f(d.y, d.x);
		return 0;
	}

	inline XY PlayerBullet::GetShootPos() {
		return XY{ std::cosf(radians), std::sinf(radians) } * cShootOffset;
	}

	inline void PlayerBullet::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res._texid_char_weapon, 1);
		q->pos = stage->camera.ToGLPos(pos) * stage->scale;
		q->anchor = { 0.5f, 0.5f };
		q->scale = stage->camera.scale * stage->scale;
		q->radians = radians;
		q->colorplus = 1.f;
		q->color = xx::RGBA8_White;
		q->texRect.data = ResTpFrames::_uvrect_char_weapon.data;
	}
}
