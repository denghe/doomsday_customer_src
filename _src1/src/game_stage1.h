#pragma once

namespace Game {

	struct Stage1 : Stage {
		xx::Shared<xx::Node> uiHUD;
		UI_HPBar uiHPBar;
		UI_CoinBar uiCoinBar;
		UI_PausePanel uiPausePanel;
		UI_ShopPanel uiShopPanel;
		xx::Ref<xx::Node> ui1, ui2, ui3;

		double coinBak{};
		BuffContainer buffsBak;
		xx::Listi32<BuffTypes> buffsLog;

		void DrawCustomUI() override;
		void Init() override;
		void Update() override;
		void RoundInit();
	};

}
