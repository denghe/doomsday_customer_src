#pragma once

namespace Game {


	struct EnvGrass : Drawable {
		static constexpr xx::FromTo<float> scaleRange{ 0.3f, 1.f };
		static constexpr xx::FromTo<float> swingRange{ -0.1f, 0.1f };
		float swingStep{};

		void Init(Stage* stage_);
		int32_t Update() override;

		static void GenGrass(Stage* stage, int32_t ratio);
	};


}
