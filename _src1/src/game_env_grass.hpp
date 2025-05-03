#pragma once

namespace Game {

	inline void EnvGrass::Init(Stage* stage_) {
		stage = stage_;
		auto& rnd = stage->rnd;
		if (rnd.Next<float>() > 0.2f) {
			frame = gLooper.res.env_grass_[2];
			scale = 1;//rnd.Next<float>(scaleRange.from, scaleRange.to) * 2;
		}
		else {
			frame = gLooper.res.env_grass_[rnd.Next<uint32_t>(2)];
			scale.x = rnd.Next<float>(scaleRange.from, scaleRange.to);
			scale.y = rnd.Next<float>(scaleRange.from, scaleRange.to);
		}
		pos.x = rnd.Next<float>(0, stage_->ground->gridSize.x * stage_->ground->size.x);
		pos.y = rnd.Next<float>(0, stage_->ground->gridSize.y * stage_->ground->size.y);
		// offsetRatio
		needFlipX = rnd.Next<bool>();
		// radius
		radians = rnd.Next<float>(swingRange.from, swingRange.to);
		// whiteColorEndTime destroyTime
		// color  todo: rnd?
		swingStep = (swingRange.to - swingRange.from) / Cfg::fps;
		if (rnd.Next<bool>()) swingStep = -swingStep;
	}

	inline int32_t EnvGrass::Update() {
		// todo: anim
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




	inline void EnvGrass::GenGrass(Stage* stage, int32_t ratio) {
		for (int i = 0; i < stage->gridSize.x * stage->gridSize.y * ratio; ++i) {
			stage->grasses.Emplace().Emplace<EnvGrass>()->Init(stage);
		}
		std::sort(stage->grasses.buf, stage->grasses.buf + stage->grasses.len, [](auto& a, auto& b) { return a->pos.y < b->pos.y; });
	}


}
