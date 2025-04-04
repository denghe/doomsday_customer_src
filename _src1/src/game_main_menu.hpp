#pragma once

namespace Game {

	inline void MainMenu::Init() {
		ui.Emplace()->Init();

		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy5m + XY{ 0, 280 }
			, Cfg::xy5a, gLooper.btnCfg_Scale5, U"new game 4.4.21.39", [&]() {
				gLooper.DelaySwitchTo<Game::Stage1>();
		});


		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy7m + XY{ 10, -10 }
			, Cfg::xy7a, gLooper.btnCfg, U"test1", [&]() {
				gLooper.DelaySwitchTo<Game::Test1>();
		});

		//ui->MakeChildren<xx::IconButton>()->Init(1, Cfg::xy5m + XY{ 0, -300 }
		//	, Cfg::xy5a, gLooper.btnCfg_Scale1Icon, gLooper.res.ui_money, U"↻114", [&]() {
		//});

		camera.SetMaxFrameSize(Cfg::unitSize);
		camera.SetScale(Cfg::defaultScale);
	}

	inline void MainMenu::Draw() {
		xx::Quad().SetFrame(gLooper.res.ui_menu).SetRotate(M_PI_2).SetScale(Cfg::width / ResTpFrames::_size_ui_menu.x).Draw();
		gLooper.DrawNode(ui);
	}
}
