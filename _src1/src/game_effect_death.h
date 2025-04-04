#pragma once

namespace Game {

	struct EffectDeath : DrawableEx {
		xx::Ref<xx::Frame> frame;
		float totalScale{}, scaleX{};
		int32_t n1{}, n2{};

		void Init(Stage* stage_, xx::Ref<xx::Frame> frame_, XY pos_ = {}, float scale_ = 1);
		int32_t Update1();
		void Update2();
		int32_t Update() override;
		void Draw() override;
	};

}
