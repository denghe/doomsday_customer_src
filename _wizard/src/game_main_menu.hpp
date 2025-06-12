#pragma once

namespace Game {

	inline void MainMenu::MakeUI() {
		ui.Emplace()->Init(0, {}, scale);

		ui->MakeChildren<xx::Button>()->Init(2, pos5 + XY{ 0, 100 }, anchor5, gLooper.btnCfg, U"new game").onClicked = [&]() {
			gLooper.DelaySwitchTo<Stage>(); 
			};
	}

	inline void MainMenu::OnWindowSizeChanged() {
		MakeUI();
	}

	inline void MainMenu::Init() {
		UpdateScale();
		MakeUI();
	}

	void MainMenu::Update() {
	}

	inline void MainMenu::Draw() {
		gLooper.DrawNode(ui);
	}

}
