﻿#pragma once

namespace Game {

	struct Stage4 : Stage {
		UI_HPBar uiHPBar;
		UI_PausePanel uiPausePanel;
		void DrawCustomUI() override;
		void Init() override;
	};

}
