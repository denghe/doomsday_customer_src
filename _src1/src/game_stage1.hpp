#pragma once

namespace Game {

	inline void Stage1::Init() {
		ui.Emplace()->Init();
		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy7m + XY{ 10, -10 }
			, Cfg::xy7a, gLooper.btnCfg, U"exit", [&]() {
			gLooper.DelaySwitchTo<Game::MainMenu>();
		});

		camera.SetMaxFrameSize(Cfg::unitSize);
		camera.SetScale(Cfg::globalScale);

		ground.Emplace()->Init(this, { 10000, 10000 });

		player.Emplace()->Init(this);
		player->pos = ResTpFrames::_size_ground_cell_ * ground->gridSize * 0.5f;

		camera.SetOriginal(player->pos);
	}

	inline void Stage1::Update() {
		// scale control
		if (gLooper.KeyDownDelay(xx::KeyboardKeys::Z, 0.02f)) {
			camera.IncreaseScale(0.1f, 5);
		}
		else if (gLooper.KeyDownDelay(xx::KeyboardKeys::X, 0.02f)) {
			camera.DecreaseScale(0.1f, 0.1f);
		}

		player->Update();
		camera.SetOriginal(player->pos);
	}

	inline void Stage1::Draw() {
		ground->Draw();
		player->Draw();
		gLooper.DrawNode(ui);

		// draw tips
		gLooper.ctcDefault.Draw({ 0, gLooper.windowSize_2.y - 5 }, "zoom: ZX  move: ASDW  ", xx::RGBA8_Green, { 0.5f, 1 });
		//gLooper.ctcDefault.Draw({ 0, gLooper.windowSize_2.y - 35 }, xx::ToString("monsters.Count() == ", monsters.Count()), xx::RGBA8_Green, { 0.5f, 1 });
	}
}
