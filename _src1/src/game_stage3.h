#pragma once

namespace Game {

	struct Stage3 : Stage {
		UI_HPBar uiHPBar;
		UI_PausePanel uiPausePanel;
		void DrawCustomUI() override;
		void Init() override;
		void OnRoundBegin() override;
		void OnRoundEnd() override;
	};

}
