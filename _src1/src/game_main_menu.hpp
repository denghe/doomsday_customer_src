#pragma once

namespace Game {

	inline void MainMenu::Init() {
		ui.Emplace()->Init();

		static constexpr auto s = Cfg::height / gLooper.res._size_ui_menu.x;	// x: rotated 90'
		ui->MakeChildren<xx::Image>()->Init(1, 0, s, 0.5, gLooper.res.ui_menu, xx::ImageRadians::PiDiv2);

		ui->MakeChildren<xx::Button>()->Init(2, Cfg::xy5m + XY{ 0, 280 }
			, Cfg::xy5a, gLooper.btnCfg_Scale4, U"new game 4.7.23.54", [&]() {
				gLooper.DelaySwitchTo<Game::Stage1>();
		});

		ui->MakeChildren<xx::Button>()->Init(2, Cfg::xy5m + XY{ 0, 0 }
			, Cfg::xy5a, gLooper.btnCfg_Scale4, U"dash test", [&]() {
				gLooper.DelaySwitchTo<Game::Stage_Dash>();
			});

		ui->MakeChildren<xx::Button>()->Init(2, Cfg::xy7m + XY{ 10, -10 }
			, Cfg::xy7a, gLooper.btnCfg, U"test1", [&]() {
				gLooper.DelaySwitchTo<Game::Test1>();
		});

		//ui->MakeChildren<xx::IconButton>()->Init(1, Cfg::xy5m + XY{ 0, -300 }
		//	, Cfg::xy5a, gLooper.btnCfg_Scale1Icon, gLooper.res.ui_money, U"↻114", [&]() {
		//});


		//auto& bg = ui->MakeChildren<xx::Image>();
		//static constexpr auto bgScale = 1080.f * 0.8f / gLooper.res._size_ui_menu.y;
		//bg->Init(1, Cfg::xy5, bgScale, Cfg::xy5a, gLooper.res.ui_menu);
		//bg->MakeChildren<xx::Label>()->Init(2, { gLooper.res._size_ui_menu.x * 0.5, gLooper.res._size_ui_menu.y * (4.f/5) }, 1.5, { 0.5, 0.5 }, xx::RGBA8_Black, "Pause");
		//bg->MakeChildren<xx::Button>()->Init(3, { gLooper.res._size_ui_menu.x * 0.5, gLooper.res._size_ui_menu.y * (1.f / 2) }, { 0.5, 0.5 }, gLooper.btnCfg_Scale2, U"Restart");
		//bg->MakeChildren<xx::Button>()->Init(3, { gLooper.res._size_ui_menu.x * 0.5, gLooper.res._size_ui_menu.y * (1.f / 4) }, { 0.5, 0.5 }, gLooper.btnCfg_Scale2, U"Continue");

		camera.SetMaxFrameSize(Cfg::unitSize);
		camera.SetScale(Cfg::defaultScale);
	}

	inline void MainMenu::Draw() {
		//xx::Quad().SetFrame(gLooper.res.ui_menu).SetRotate(M_PI_2).SetScale(Cfg::width / ResTpFrames::_size_ui_menu.x).Draw();
		gLooper.DrawNode(ui);
	}
}
