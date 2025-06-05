#pragma once

namespace Game {

	XX_INLINE void EffectExplosion::Init(XY pos_, float scale_, xx::RGBA8 color_) {
		Init(&gLooper.res.global_explosion_, 20.f, pos_, scale_, color_);
	}

	XX_INLINE void EffectExplosion::Init(xx::Listi32<xx::Ref<xx::Frame>>* frames_, float frameFPS, XY pos_, float scale_, xx::RGBA8 color_) {
		frames = frames_;
		frameDelay = Cfg::frameDelay * frameFPS;
		pos = pos_;
		scale = scale_;
		color = color_;
		radians = gLooper.rnd.Next<float>(-M_PI, M_PI);
		lightRadius = 1.f;
	}

	XX_INLINE int32_t EffectExplosion::Update() {
		frameIndex += frameDelay;
		if (frameIndex >= frames->len) return 1;
		lightRadius *= 0.98f;
		return 0;
	}

	XX_INLINE void EffectExplosion::Draw(Stage* stage) {
		auto& frame = frames->At((int32_t)frameIndex);
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(frame->tex, 1);
		q->pos = stage->camera.ToGLPos(pos);
		q->anchor = { 0.5f, 0.5f };
		q->scale = stage->camera.scale * scale;
		q->radians = radians;
		q->colorplus = 1.f;
		q->color = color;
		q->texRect.data = frame->textureRect.data;
	}

	XX_INLINE void EffectExplosion::DrawLight(Stage* stage) {
		auto& frame = gLooper.res.ef_light64;
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(frame->tex, 1);
		q->pos = stage->camera.ToGLPos(pos);
		q->anchor = { 0.5f, 0.5f };
		q->scale = stage->camera.scale * scale * 3.f * lightRadius;
		q->radians = radians;
		q->colorplus = 1.f;
		q->color = color;
		q->texRect.data = frame->textureRect.data;
	}

};
