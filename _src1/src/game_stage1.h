#pragma once

namespace Game {

	struct Stage1 : Stage {
		UI_HPBar uiHPBar;
		UI_CoinBar uiCoinBar;
		UI_PausePanel uiPausePanel;
		UI_ShopPanel uiShopPanel;
		void DrawCustomUI() override;
		void Init() override;
	};

}
