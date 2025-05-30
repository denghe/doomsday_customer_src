﻿#pragma once

namespace Game {

	UI_PausePanel::operator bool() const {
		return ui;
	}

	void UI_PausePanel::Init(Creature* creature_) {

		// todo: draw player stat & buffs

		ui.Emplace()->Init();
		ui->MakeChildren<xx::SwallowButton>()->Init(1).onClicked = [] {};		// avoid user click other buttons

		static constexpr auto bgScale = 1080.f * 0.8f / gLooper.res._size_ui_paper.y;
		auto& img = ui->MakeChildren<xx::Image>();
		img->Init(2, Cfg::xy5, bgScale, Cfg::xy5a, gLooper.res.ui_paper);

		static constexpr float x = gLooper.res._size_ui_paper.x * 0.5f;
		static constexpr float h = gLooper.res._size_ui_paper.y;

		img->MakeChildren<xx::Label>()->Init(3, { x, h * 4.f / 5.f }, 1.5f, 0.5f, xx::RGBA8_Black, U"暂停");

		img->MakeChildren<xx::Button>()->Init(3, { x, h * 1.f / 2.f }, 0.5f, gLooper.btnCfg2, U"回主菜单").onClicked = [this] {
			gLooper.DelaySwitchTo<Game::MainMenu>();
		};

		img->MakeChildren<xx::Button>()->Init(3, { x, h * 1.f / 4.f }, 0.5f, gLooper.btnCfg2, U"继续").onClicked = [this] {
			ui.Reset();										// close popup panel
		};
	}

	void UI_PausePanel::TryDraw() {
		if (ui) {
			gLooper.DrawNode(ui);
		}
	}
}
