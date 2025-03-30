#pragma once

namespace Game {

	inline void MainMenu::Init() {
		ui.Emplace()->Init();
		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy5m + XY{ 0, 0 }
			, Cfg::xy5a, gLooper.btnCfg, U"new game", [&]() {
				gLooper.DelaySwitchTo<Game::Stage1>();
		});

		camera.SetMaxFrameSize(Cfg::ItemBaseSize);
		camera.SetScale(Cfg::Scale);
		//camera.SetOriginal(mapSize_2f);
	}

	inline void MainMenu::Draw() {
		gLooper.DrawNode(ui);
	}
}
