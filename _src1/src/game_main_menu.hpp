#pragma once

namespace Game {

	inline void MainMenu::Init() {
		ui.Emplace()->Init();

		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy5m + XY{ 0, 60 }
			, Cfg::xy5a, gLooper.btnCfg2, U"new game ( 2025.4.3 11:58 )", [&]() {
				gLooper.DelaySwitchTo<Game::Stage1>();
		});

		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy5m + XY{ 0, -60 }
			, Cfg::xy5a, gLooper.btnCfg3, U"↻ $114", [&]() {
			// todo: icon button
		});

		camera.SetMaxFrameSize(Cfg::unitSize);
		camera.SetScale(Cfg::defaultScale);
	}

	inline void MainMenu::Draw() {
		xx::Quad().SetFrame(gLooper.res.ui_menu).SetRotate(M_PI_2).SetScale(Cfg::width / ResTpFrames::_size_ui_menu_.x).Draw();
		gLooper.DrawNode(ui);
	}
}
