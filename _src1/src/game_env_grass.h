#pragma once

namespace Game {

	struct EnvGrass : Drawable {
		xx::FromTo<float> swingRange;
		float swingStep{};

		void Init(Stage* stage_, xx::Ref<xx::Frame> frame_, XY pos_, XY scale_, xx::FromTo<float> swingRange_);
		int32_t Update() override;
	};

}
