#pragma once

namespace Game {

	struct Stage1 : Stage {
		UI_HPBar uiHPBar;
		xx::Shared<xx::Node> pausePanel;
		void DrawCustomUI() override;
		void Init() override;
	};

}
