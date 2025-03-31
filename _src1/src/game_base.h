#pragma once

namespace Game {

	struct StageBase : xx::SceneBase {
		xx::Camera camera;
		xx::Shared<xx::Node> ui;

		XYi cellSize{};	// cell's pixel size
		XYi gridSize{};	// grid's num cols rows
		XY mapSize{};	// cellSize * gridSize( grid pixel size )

		int32_t time{};	// frame number

		virtual XY GetPlayerBornPos();
		void ForceLimit(XY& pos);
	};

	struct Drawable {
		StageBase* ownerStage{};
		xx::XY pos{}, scale{};
		virtual void Draw() {};
		virtual ~Drawable() {};
	};
}
