#pragma once

namespace Game {

	inline void WarningDash::Init(Stage* stage_, xx::XY pos_, xx::XY targetPos_, xx::XY size_, float dashDelaySecond_, float fadeOutSecond_) {
		assert(dashDelaySecond <= 0);

		stage = stage_;
		pos = pos_;
		dashDelaySecond = dashDelaySecond_;
		centerScaleStep = 1.0f / (dashDelaySecond_ * Cfg::fps);
		aStep = 255 / (fadeOutSecond_ * Cfg::fps);

		xx::XY size = size_;
		auto textScale = 2;

		s9.frame = gLooper.res.warning2;
		s9.centerOffset = { 0, -(size.y - 4 * textScale) / 2 };
		s9.centerAnchor = { 0, 0.5 };
		s9.centerScale = { 1, 1 };
		s9.texScale = {textScale, textScale};
		s9.center = { 2,2,2,2 };
		s9.size = size;
		s9.anchor = { 0, 0.5f };

		auto d = targetPos_ - pos;
		auto dd = d.x * d.x + d.y * d.y;
		auto mag = std::sqrtf(dd);
		auto norm = d / mag;
		auto radians = std::atan2f(norm.y, norm.x);
		s9.radians = radians;
	}

	inline int32_t WarningDash::Update() {
		XX_BEGIN(n);
		for (s9.centerScale.y = 0.f; s9.centerScale.y <= 1.f; s9.centerScale.y += centerScaleStep) {
			XX_YIELD_I(n);
		}
		for (a = 255.f; a >= 0; a -= aStep) {
			s9.color = xx::RGBA8{ 255, uint8_t(a), uint8_t(a), uint8_t(a) };
			XX_YIELD_I(n);
		}
		XX_END(n);
		return 1;
	}

	inline void WarningDash::Draw() {
		s9.pos = stage->camera.ToGLPos(pos);
		s9.scale = stage->camera.scale;
		s9.Draw();
	}
}
