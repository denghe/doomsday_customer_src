#pragma once

namespace Game {

	struct Stage2 : Stage {
		UI_HPBar uiHPBar;
		void DrawCustomUI() override;
		void Init() override;
	};

}
