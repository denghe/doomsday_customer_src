#pragma once

namespace Game {

	struct Stage1 : Stage {
		UI_HPBar uiHPBar;
		void DrawCustomUI() override;
		void Init() override;
	};

}
