#pragma once

namespace Game {

	struct DashEarlyWarning : DrawableEx {
		xx::Scale9 s9;
		int32_t n{};
		Stage* stage;
		int32_t dashDelay{};
		float centerScaleStep{};

		void Init(Stage* stage_, xx::XY pos_, xx::XY targetPos_, int32_t width_, int32_t length_, int32_t dashDelay_);
		int32_t Update() override;
		void Draw() override;
	};

}
