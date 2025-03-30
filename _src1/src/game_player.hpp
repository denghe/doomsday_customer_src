#pragma once

namespace Game {

	void Player::Anim_Idle() {
		static constexpr float cTotal{ 2.f };
		static constexpr float cRange{ 0.07f };
		static constexpr float cTimeSpan{ 0.3f };
		static constexpr float cInc{ cRange / (Cfg::fps * cTimeSpan) };
		static constexpr float cEnd{ 1.f - cRange };

		XX_BEGIN(lineNumber_Anim_Idle);
		for (scale.y = 1.f; scale.y >= cEnd; scale.y -= cInc) {
			scale.x = cTotal - scale.y;
			XX_YIELD(lineNumber_Anim_Idle);
		}
		for (; scale.y <= 1.f; scale.y += cInc) {
			scale.x = cTotal - scale.y;
			XX_YIELD(lineNumber_Anim_Idle);
		}
		XX_YIELD_TO_BEGIN(lineNumber_Anim_Idle);
		XX_END(lineNumber_Anim_Idle);
	}

	inline void Player::Init(Stage1* owner_) {
		owner = owner_;
		frame = gLooper.res.player1;
	}

	inline void Player::Update() {
		if (auto inc = gLooper.GetKeyboardMoveInc(); inc.has_value()) {
			pos += inc->second * (cMoveSpeed / Cfg::fps);
		}
		Anim_Idle();
	}

	inline void Player::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(frame->tex->GetValue(), 1);
		q->pos = owner->camera.ToGLPos(pos);
		q->anchor = { 0.5f, 0.f };
		q->scale = scale * Cfg::globalScale * owner->camera.scale;
		q->radians = 0;
		q->colorplus = 1;
		q->color = xx::RGBA8_White;
		q->texRect.data = frame->textureRect.data;
	}
}
