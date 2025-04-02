#pragma once

namespace Game {

	inline void MainMenu::Init() {
		ui.Emplace()->Init();
		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy5m + XY{ 0, 0 }
			, Cfg::xy5a, gLooper.btnCfg, U"new game ( 2025.4.2 14.51 )", [&]() {
				gLooper.DelaySwitchTo<Game::Stage1>();
		});

		camera.SetMaxFrameSize(Cfg::unitSize);
		camera.SetScale(Cfg::globalScale);
		//camera.SetOriginal(mapSize_2f);
	}

	inline void MainMenu::Draw() {
		gLooper.DrawNode(ui);
	}
}
