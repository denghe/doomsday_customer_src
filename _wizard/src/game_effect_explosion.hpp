#pragma once

namespace Game {

	XX_INLINE void EffectExplosion::Init(xx::Listi32<xx::Ref<xx::Frame>>* frames_, float frameFPS, XY pos_, float scale_) {
		frames = frames_;
		pos = pos_;
		frameIndex = 0;
		scale = scale_;
		frameDelay = Cfg::frameDelay * frameFPS;
		lightRadius = 32.f * 5.f;
		radians = gLooper.rnd.Next<float>(-M_PI, M_PI);
	}

	XX_INLINE int32_t EffectExplosion::Update() {
		frameIndex += frameDelay;
		if (frameIndex >= frames->len) return 1;
		lightRadius *= 0.99f;
		return 0;
	}

	XX_INLINE void EffectExplosion::Draw(Stage* stage) {
		auto& frame = frames->At((int32_t)frameIndex);
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(frame->tex, 1);
		q->pos = stage->camera.ToGLPos(pos) * stage->scale;
		q->anchor = { 0.5f, 0.5f };
		q->scale = stage->camera.scale * stage->scale;
		q->radians = radians;
		q->colorplus = 1;
		q->color = xx::RGBA8_White;
		q->texRect.data = frame->textureRect.data;
	}

};
