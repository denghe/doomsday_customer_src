#pragma once

namespace Game {

	inline void DashEarlyWarning::Init(Stage* stage_, xx::XY pos_, xx::XY targetPos_, int32_t width_ ,int32_t length_, int32_t dashDelay_) {
		stage = stage_;
		pos = pos_;
		targetPos = targetPos_;
		dashDelay = dashDelay_;

		xx::XY size = {length_, width_ };
		auto textScale = 2;

		s9.frame = gLooper.res.warning2;
		s9.centerOffset = { 0, -(size.y - 4 * textScale) / 2 };
		s9.centerAnchor = { 0, 0.5 };
		s9.centerScale = { 1, 1 };
		s9.texScale = {textScale, textScale};
		s9.center = { 2,2,2,2 };
		s9.size = size;
		s9.anchor = { 0, 0.5f };

		auto d = targetPos - pos;
		auto dd = d.x * d.x + d.y * d.y;
		auto mag = std::sqrtf(dd);
		auto norm = d / mag;
		auto radians = std::atan2f(norm.y, norm.x);
		s9.radians = radians;
	}

	inline int32_t DashEarlyWarning::Update() {
		XX_BEGIN(n);
		for (s9.centerScale.y = 0.f; s9.centerScale.y <= 1.f; s9.centerScale.y += 1.0 / dashDelay ) {
			XX_YIELD_I(n);
		}
		for (s9.color.a = 255; s9.color.a >= 0; s9.color.a--) {
			XX_YIELD_I(n);
		}
		XX_END(n);
		return 1;
	}

	inline void DashEarlyWarning::Draw() {
		s9.pos = stage->camera.ToGLPos(pos);
		s9.scale = stage->camera.scale;
		s9.Draw();
	}
}
