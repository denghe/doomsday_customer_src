#pragma once

namespace Game {

	struct WarningDash : Drawable {
		xx::Scale9 s9;
		int32_t n{};
		float a{};
		float aStep{};
		float  dashDelaySecond{};
		float centerScaleStep{};

		void Init(Stage* stage_, xx::XY pos_, xx::XY targetPos_, xx::XY size_, float dashDelaySecond_, float fadeOutSecond_);
		int32_t Update() override;
		void Draw() override;
	};

}
