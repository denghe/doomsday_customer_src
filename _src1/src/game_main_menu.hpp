#pragma once

namespace Game {

	inline void MainMenu::Init() {
		ui.Emplace()->Init();

		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy5m + XY{ 0, 80 }
			, Cfg::xy5a, gLooper.btnCfg_Scale5, U"new game 4.3.17.05", [&]() {
				gLooper.DelaySwitchTo<Game::Stage1>();
		});

		ui->MakeChildren<xx::IconButton>()->Init(1, Cfg::xy5m + XY{ 0, -150 }
			, Cfg::xy5a, gLooper.btnCfg_Scale5Icon, gLooper.res.ui_money, U"$114", [&]() {
		});

		ui->MakeChildren<xx::IconButton>()->Init(1, Cfg::xy5m + XY{ 0, -300 }
			, Cfg::xy5a, gLooper.btnCfg_Scale1Icon, gLooper.res.ui_money, U"↻114", [&]() {
		});

		camera.SetMaxFrameSize(Cfg::unitSize);
		camera.SetScale(Cfg::defaultScale);
	}

	inline void MainMenu::Draw() {
		xx::Quad().SetFrame(gLooper.res.ui_menu).SetRotate(M_PI_2).SetScale(Cfg::width / ResTpFrames::_size_ui_menu_.x).Draw();
		gLooper.DrawNode(ui);
	}
}
