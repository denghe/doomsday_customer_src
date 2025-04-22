#pragma once

namespace Game {

	inline void MainMenu::Init() {
		ui.Emplace()->Init();

		static constexpr auto s = Cfg::height / gLooper.res._size_ui_paper.x;	// x: rotated 90'
		ui->MakeChildren<xx::Image>()->Init(1, 0, s, 0.5, gLooper.res.ui_paper, xx::ImageRadians::PiDiv2);

		ui->MakeChildren<xx::Button>()->Init(2, Cfg::xy5m + XY{ 0, 280 }, Cfg::xy5a, gLooper.btnCfg4, U"new game 4.22.15.58").onClicked = [&]() {
			gLooper.DelaySwitchTo<Game::Stage1>();
		};

		ui->MakeChildren<xx::Button>()->Init(2, Cfg::xy5m + XY{ 0, 0 }, Cfg::xy5a, gLooper.btnCfg4, U"dash test").onClicked = [&]() {
			gLooper.DelaySwitchTo<Game::Stage_Dash>();
		};

		ui->MakeChildren<xx::Button>()->Init(2, Cfg::xy7m + XY{ 10, -10 }, Cfg::xy7a, gLooper.btnCfg, U"test1").onClicked = [&]() {
			gLooper.DelaySwitchTo<Game::Test1>();
		};

		ui->MakeChildren<xx::Button>()->Init(2, Cfg::xy7m + XY{ 10, -70 }, Cfg::xy7a, gLooper.btnCfg, U"test2").onClicked = [&]() {
			gLooper.DelaySwitchTo<Game::Test2>();
		};

		ui->MakeChildren<xx::Button>()->Init(2, Cfg::xy7m + XY{ 10, -130 }, Cfg::xy7a, gLooper.btnCfg, U"test3").onClicked = [&]() {
			gLooper.DelaySwitchTo<Game::Test3>();
		};

		ui->MakeChildren<xx::Button>()->Init(2, Cfg::xy7m + XY{ 10, -190 }, Cfg::xy7a, gLooper.btnCfg, U"test4").onClicked = [&]() {
			gLooper.DelaySwitchTo<Game::Test4>();
		};

		camera.SetMaxFrameSize(Cfg::unitSize);
		camera.SetScale(Cfg::defaultScale);
	}

	inline void MainMenu::Draw() {
		gLooper.DrawNode(ui);
	}
}
