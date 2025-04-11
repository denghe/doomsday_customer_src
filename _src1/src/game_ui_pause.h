#pragma once

namespace Game {

	// modal panel
	struct Stage;
	struct UI_PausePanel {
		Stage* stage{};
		xx::Shared<xx::Node> ui;
		void Init(Stage* stage_);
		void Popup();
		void TryDraw();
	};

}
