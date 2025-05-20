#pragma once

namespace Game {

	inline void EnvGrass::Init(Stage* stage_, xx::Ref<xx::Frame> frame_, XY pos_, XY scale_, xx::FromTo<float> swingRange_) {
		stage = stage_;
		frame = std::move(frame_);
		pos = pos_;
		scale = scale_;
		// offsetRatio
		auto& rnd = stage->rnd;
		needFlipX = rnd.Next<bool>();
		// radius
		radians = rnd.Next<float>(swingRange.from, swingRange.to);
		// whiteColorEndTime destroyTime
		// color  todo: rnd?
		swingRange = swingRange_;
		swingStep = (swingRange.to - swingRange.from) / Cfg::fps;
		if (rnd.Next<bool>()) swingStep = -swingStep;
	}

	inline int32_t EnvGrass::Update() {
		radians += swingStep;
		if (swingStep > 0) {
			if (radians > swingRange.to) {
				radians = swingRange.to;
				swingStep = -swingStep;
			}
		}
		else {
			if (radians < swingRange.from) {
				radians = swingRange.from;
				swingStep = -swingStep;
			}
		}
		// todo: player & monster neighbor check & change radians
		return 0;
	}

}
