#pragma once

namespace Game {

	struct Stage5 : Stage {
		UI_HPBar uiHPBar;
		void DrawCustomUI() override;
		void Init() override;
	};

}
