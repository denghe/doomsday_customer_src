﻿#pragma once

namespace Game {

	struct StageBase : xx::SceneBase {
		xx::Camera camera;
		xx::Shared<xx::Node> ui;

		virtual XY GetPlayerBornPos();
	};

	struct Drawable {
		StageBase* ownerStage{};
		xx::XY pos{}, scale{};
		virtual void Draw() {};
		virtual ~Drawable() {};
	};
}
