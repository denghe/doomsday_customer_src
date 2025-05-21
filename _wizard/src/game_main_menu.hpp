#pragma once

namespace Game {

	inline void MainMenu::MakeUI() {
		ui.Emplace()->Init(0, {}, scale);
		ui->MakeChildren<xx::Button>()->Init(2, pos5 + XY{ 0, 0 }, anchor5
			, gLooper.btnCfg, U"new game").onClicked = [&]() {
			};
		ui->MakeChildren<xx::Button>()->Init(2, pos7 + XY{ 10, -10 }, anchor7
			, gLooper.btnCfg, U"test1").onClicked = [&]() {
				gLooper.DelaySwitchTo<Test1>();
			};
	}

	inline void MainMenu::Init() {
		UpdateScale();
		MakeUI();
	}

	inline void MainMenu::Draw() {
		gLooper.DrawNode(ui);
	}

	inline void MainMenu::OnWindowSizeChanged() {
		MakeUI();
	}
}
