#pragma once

namespace Game {

	struct Stage4 : Stage {
		UI_HPBar uiHPBar;
		void DrawCustomUI() override;
		void Init() override;
	};

}
