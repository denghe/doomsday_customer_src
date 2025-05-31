#pragma once

namespace Game {

	struct EffectExplosion {
		xx::Listi32<xx::Ref<xx::Frame>>* frames{};
		XY pos{};
		float frameIndex{}, scale{}, frameDelay{}, radians{};
		float colorPlus{};
		xx::RGBA8 color{};
		xx::RGBA8 lightColor{};
		float lightColorPlus{};
		XY lightRadius{};

		void Init(xx::Listi32<xx::Ref<xx::Frame>>* frames_, float frameFPS, XY pos_, float scale_, xx::RGBA8 color_, float colorPlus_, xx::RGBA8 lightColor_, float lightColorPlus_, XY lightRadius_);
		void Init(XY pos_, float scale_, xx::RGBA8 color_, float colorPlus_, xx::RGBA8 lightColor_, float lightColorPlus_, XY lightRadius_);
		int32_t Update();
		void Draw(struct Stage* stage);
	};

}
