﻿#pragma once

namespace Game {

	struct Stage2 : Stage {
		UI_HPBar uiHPBar;
		UI_PausePanel uiPausePanel;
		void DrawCustomUI() override;
		void Init() override;
	};

}
