#pragma once

namespace Game {

	struct Stage;

	// modal panel
	struct UI_ShopPanel {
		Stage* stage{};
		xx::Shared<xx::Node> ui;
		void Init(Stage* stage_);
		void Popup();
		void TryDraw();
	};

}
