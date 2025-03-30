#pragma once

namespace Game {

	void Player::Anim_Idle() {
		static constexpr float sRange{ 0.3f };
		static constexpr float sTimeSpan{ 0.5f };
		static constexpr float sInc{ sRange / (Cfg::fps * sTimeSpan) };
		static constexpr float sEnd{ 1.f - sRange };
		XX_BEGIN(lineNumber);
		for (sy = 1.f; sy >= sEnd; sy -= sInc) {
			sx = 2.f - sy;
			XX_YIELD(lineNumber);
		}
		for (; sy <= 1.f; sy += sInc) {
			sx = 2.f - sy;
			XX_YIELD(lineNumber);
		}
		XX_YIELD_TO_BEGIN(lineNumber);
		XX_END(lineNumber);
	}

	inline void Player::Init(Stage1* owner_) {
		owner = owner_;
		frame = gLooper.res.player1;
	}

	inline void Player::Update() {
		Anim_Idle();
	}

	inline void Player::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(frame->tex->GetValue(), 1);
		q->pos = owner->camera.ToGLPos(pos);
		q->anchor = { 0.5f, 0.5f };
		q->scale = XY{ sx, sy } * Cfg::Scale * owner->camera.scale;
		q->radians = 0;
		q->colorplus = 1;
		q->color = xx::RGBA8_White;
		q->texRect.data = frame->textureRect.data;
	}
}
