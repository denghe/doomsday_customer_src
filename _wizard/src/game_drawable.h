#pragma once

namespace Game {

	struct alignas(8) Drawable {
		struct Stage* stage{};								// stage's life > this
		xx::Ref<xx::Frame> frame;

		xx::XY pos{};										// pivot position
		xx::XY scale{ 1.f, 1.f };							//
		xx::XY offsetRatio{ 0.5f, 0.3f};					// pos + size * scale * offsetRatio == center pos
		bool needFlipX{};
		float radius{}, radians{};							// for logic
		int32_t whiteColorEndTime{};						// for hurt effect
		int32_t destroyTime{};								// max life cycle for bug issue
		xx::RGBA8 color{xx::RGBA8_White};
		// todo

		virtual int32_t Update() { return 0; };				// return !0 mean need Release/Delete/Remove
		virtual void Draw();
		virtual ~Drawable() {};
	};

}
