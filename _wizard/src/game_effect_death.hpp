#pragma once

namespace Game {

	inline void EffectDeath::Init(Stage* stage_, xx::Ref<xx::Frame> frame_, XY pos_, float scale_) {
		stage = stage_;
		frame = std::move(frame_);
		pos = pos_;
		scale = scale_;
		totalScale = 1;
		scaleX = 1;
	}

	XX_INLINE int32_t EffectDeath::Update1() {
		XX_BEGIN(n1);
		for (totalScale = 1.f; totalScale >= 0.f; totalScale -= 1.f / (Cfg::fps * 0.7)) {
			XX_YIELD_I(n1);
		}
		XX_END(n1);
		return 1;
	}

	XX_INLINE void EffectDeath::Update2() {
		XX_BEGIN(n2);
		for (scaleX = 1.f; scaleX > -1.f; scaleX -= 1.f / (Cfg::fps * 0.06)) {
			XX_YIELD(n2);
		}
		for (scaleX = -1.f; scaleX < 1.f; scaleX += 1.f / (Cfg::fps * 0.06)) {
			XX_YIELD(n2);
		}
		XX_YIELD_TO_BEGIN(n2);
		XX_END(n2);
	}

	inline int32_t EffectDeath::Update() {
		if (Update1()) return 1;
		Update2();
		return 0;
	}

	inline void EffectDeath::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(frame->tex, 1);
		q->pos = stage->camera.ToGLPos(pos);
		q->anchor = frame->anchor.has_value() ? frame->anchor.value() : XY{ 0.5f, 0.5f };
		q->scale = XY{ totalScale * scaleX, totalScale } * scale * stage->camera.scale;
		q->radians = 0;
		q->colorplus = 1.f;
		q->color = xx::RGBA8_White;
		q->texRect.data = frame->textureRect.data;
	}

}
