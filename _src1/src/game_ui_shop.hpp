#pragma once

namespace Game {
	void UI_ShopPanel::Init(Stage* stage_) {
		stage = stage_;
	}

	void UI_ShopPanel::Popup() {
		stage->paused = true;								// pause

		ui.Emplace()->Init();
		ui->MakeChildren<xx::SwallowButton>()->Init(1).onClicked = [] {};		// avoid user click other buttons

		auto& img = ui->MakeChildren<xx::Image>();
		static constexpr auto bgScale = 1080.f * 0.95f / gLooper.res._size_ui_paper.x;
		img->Init(2, Cfg::xy5, bgScale, Cfg::xy5a, gLooper.res.ui_paper, xx::ImageRadians::PiDiv2);

		float posX = gLooper.res._size_ui_paper.y * 0.5f;
		//img->MakeChildren<xx::Label>()->Init(3, { posX, gLooper.res._size_ui_paper.y * (4.f / 5.f) }, 1.5f, 0.5f, xx::RGBA8_Black, "Pause");
		//img->MakeChildren<xx::Button>()->Init(3, { posX, gLooper.res._size_ui_paper.y * (1.f / 2.f) }, .5f, gLooper.btnCfg_Scale2, U"Restart")
		//	.onClicked = [this] {
		//	gLooper.DelaySwitchTo<Game::MainMenu>();
		//};
		img->MakeChildren<xx::Button>()->Init(3, { posX, gLooper.res._size_ui_paper.y * (1.f / 4.f) }, .5f, gLooper.btnCfg_Scale2, U"Continue").onClicked = [this] {
			stage->paused = false;							// resume
			ui.Reset();										// close popup panel
		};
	}

	void UI_ShopPanel::TryDraw() {
		if (ui) {
			gLooper.DrawNode(ui);
		}
	}

}
