#pragma once

namespace Game {

	struct Monster : Drawable {
		int32_t indexAtItems{ -1 }, indexAtCells{ -1 };
		Monster* prev{}, * next{};
		xx::Ref<xx::Frame> frame;
		XY lightRadius{};
		float radius{};
		float radians{};
		XY moveInc{};

		void Init(Stage* stage_, xx::Ref<xx::Frame> frame_, int32_t bornPosIndex);
		int32_t Update() override;
		void Draw() override;
	};

}
