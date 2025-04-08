#pragma once

namespace Game {

	struct Stage3 : Stage {
		UI_HPBar uiHPBar;
		void DrawCustomUI() override;
		void Init() override;
	};

}
