#pragma once

namespace Game {

	struct EffectExplosion {
		xx::Listi32<xx::Ref<xx::Frame>>* frames{};
		XY pos{};
		float frameIndex{}, scale{}, frameDelay{}, lightRadius{}, radians{};
		void Init(xx::Listi32<xx::Ref<xx::Frame>>* frames_, float frameFPS, XY pos_, float scale_);
		int32_t Update();
		void Draw(struct Stage* stage);
	};

}
