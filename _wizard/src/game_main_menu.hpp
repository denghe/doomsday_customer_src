#pragma once

namespace Game {

	inline void MainMenu::Init() {
		ui.Emplace()->Init();

		ui->MakeChildren<xx::Button>()->Init(2, Cfg::xy5m + XY{ 0, 280 }, Cfg::xy5a, gLooper.btnCfg, U"new game").onClicked = [&]() {
		};

		//if (lastWindowSize != gLooper.windowSize) {
		//	OnWindowSizeChanged();
		//	lastWindowSize = gLooper.windowSize;
		//}
	}

	inline void MainMenu::Draw() {
		gLooper.DrawNode(ui);
	}
}
