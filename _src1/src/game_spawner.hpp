#pragma once

namespace Game {

	template<typename F> void Spawner::Init(Stage* stage_, XY const& pos_, float scale_, float delaySeconds, F&& createFunc) {
		stage = stage_;
		frame = gLooper.res.warning1a;

		pos = pos_;
		scale = scale_;
		elapsedTime =  stage_->time + int32_t(delaySeconds * Cfg::fps);
		onDispose = std::forward<F>(createFunc);
	}

	inline int32_t Spawner::Update() {
		if (stage->time >= elapsedTime) {
			onDispose(stage, pos);
			return 1;
		}

		XX_BEGIN(n1);
		visible = true;
		while (true) {
			flashTime = stage->time + int32_t(Cfg::fps * 0.1f);
			while (stage->time < flashTime) {
				XX_YIELD_I(n1);
			}
			visible = !visible;
		}
		XX_END(n1);
		return 0;
	}

	inline void Spawner::Draw() {
		if (visible) {
			auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(frame->tex, 1);
			q->pos = stage->camera.ToGLPos(pos);
			q->anchor = *frame->anchor;
			q->scale = scale * stage->camera.scale;
			q->radians = 0;
			q->colorplus = 1.f;
			q->color = xx::RGBA8_Red;
			q->texRect.data = frame->textureRect.data;
		}
	}

}
