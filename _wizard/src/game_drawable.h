#pragma once

namespace Game {

	struct alignas(8) Drawable {
		struct Stage* stage{};
		xx::XY pos{};										// pivot position. y for order check
		virtual int32_t Update() { return 0; };				// return !0 mean need Release/Delete/Remove
		virtual void Draw() {};
		virtual void DrawLight(float colorPlus_) {};
		virtual ~Drawable() {};
	};

}
