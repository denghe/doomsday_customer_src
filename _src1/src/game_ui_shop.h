#pragma once

namespace Game {

	// modal panel
	struct Stage;
	struct UI_ShopPanel {
		Stage* stage{};
		xx::Shared<xx::Node> ui;
		void Init(Stage* stage_);
		void Popup();
		void TryDraw();
	};

}
