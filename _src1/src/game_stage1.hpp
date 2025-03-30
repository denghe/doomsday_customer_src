#pragma once

namespace Game {

	inline void Stage1::Init() {
		ui.Emplace()->Init();
		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy7m + XY{ 10, -10 }
			, Cfg::xy7a, gLooper.btnCfg, U"exit", [&]() {
			gLooper.DelaySwitchTo<Game::MainMenu>();
		});

		camera.SetMaxFrameSize(Cfg::ItemBaseSize);
		camera.SetScale(Cfg::Scale);
		//camera.SetOriginal(mapSize_2f);

		player.Emplace()->Init(this);
	}

	inline void Stage1::Update() {
		player->Update();
	}

	inline void Stage1::Draw() {
		player->Draw();
		gLooper.DrawNode(ui);
	}
}
