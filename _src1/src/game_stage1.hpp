#pragma once

namespace Game {

	inline void Stage1::Init() {
		ui.Emplace()->Init();
		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy7m + XY{ 10, -10 }
			, Cfg::xy7a, gLooper.btnCfg, U"exit", [&]() {
			gLooper.DelaySwitchTo<Game::MainMenu>();
		});

		camera.SetMaxFrameSize(Cfg::itemBaseSize);
		camera.SetScale(Cfg::globalScale);
		//camera.SetOriginal(mapSize_2f);

		player.Emplace()->Init(this);
	}

	inline void Stage1::Update() {
		player->Update();
	}

	inline void Stage1::Draw() {
		player->Draw();
		gLooper.DrawNode(ui);
		// draw tips
		gLooper.ctcDefault.Draw({ 0, gLooper.windowSize_2.y - 5 }, "move: ASDW", xx::RGBA8_Green, { 0.5f, 1 });
		//gLooper.ctcDefault.Draw({ 0, gLooper.windowSize_2.y - 35 }, xx::ToString("monsters.Count() == ", monsters.Count()), xx::RGBA8_Green, { 0.5f, 1 });
	}
}
