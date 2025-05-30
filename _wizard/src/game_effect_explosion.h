#pragma once

namespace Game {

	struct EffectExplosion {
		xx::Listi32<xx::Ref<xx::Frame>>* frames{};
		XY pos{};
		float frameIndex{}, scale{}, frameDelay{}, lightRadius{}, radians{};
		xx::RGBA8 color{};

		void Init(xx::Listi32<xx::Ref<xx::Frame>>* frames_, float frameFPS, XY pos_, float scale_, xx::RGBA8 color_ = xx::RGBA8_White);
		int32_t Update();
		void Draw(struct Stage* stage);
	};

}
