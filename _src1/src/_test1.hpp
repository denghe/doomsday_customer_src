#pragma once

namespace Game {

	inline void Test1::Init() {
		ui.Emplace()->Init();

		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy7m + XY{ 10, -10 }
			, Cfg::xy7a, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
		});

		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy8m + XY{ 0, -10 }
			, Cfg::xy8a, gLooper.btnCfg, U"test1", [&]() {
		});

		gridSize = {60, 60};
		mapSize = 128 * gridSize;

		ground.Emplace()->Init(this, mapSize, gLooper.res.ground_cell2);
		player.Emplace<Player_1>()->Init(this);

		camera.scale = Cfg::defaultScale;
		camera.mapSize = mapSize;
		camera.newOriginal = camera.original = mapSize * 0.5f;
	}

	inline void Test1::Update() {
		player->Update();
		camera.SetOriginal<true>(player->pos, camera.ToLogicPos(gLooper.mouse.pos));
		camera.Update();
		++time;
	}

	inline void Test1::Draw() {
		ground->Draw();
		player->Draw();
		gLooper.DrawNode(ui);
	}
}
