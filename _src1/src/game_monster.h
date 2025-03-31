#pragma once

namespace Game {

	struct Monster : Drawable {
		XY pos{};
		float radius{};
		int32_t indexAtItems{ -1 }, indexAtCells{ -1 };
		Monster* prev{}, * next{};

		virtual int32_t Update() { return 0; };
	};

}
