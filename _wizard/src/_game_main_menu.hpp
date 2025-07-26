#pragma once

namespace Game {

	inline void MainMenu::MakeUI() {
		ui.Emplace()->Init(0, {}, scale);

		auto& cn = gLooper.btnCfgNormal;
		auto& ch = gLooper.btnCfgHighlight;
		{
			auto& b = ui->MakeChildren<xx::FocusButton>()->Init(2, pos5 + XY{ 0, 0 }, 0.5f
				, cn, ch, U"new game");
			b.onFocus = [] { gLooper.sound.Play(gLooper.res_sound_button_1); };
			b.onClicked = [] { gLooper.DelaySwitchTo<Stage>(); };
		}
		{
			auto& b = ui->MakeChildren<xx::FocusButton>()->Init(2, pos5 + XY{ 0, -100 }, 0.5f
				, cn, ch, U"test2");
			b.onFocus = [] { gLooper.sound.Play(gLooper.res_sound_button_1); };
			b.onClicked = [] { gLooper.DelaySwitchTo<Test2>(); };
		}

#if 0
		{
			static bool v{};
			auto& b = ui->MakeChildren<xx::FocusButton>()->Init(2, pos5 + XY{ 0, 200 }, 0.5f
				, cn, ch, U"btn4", U"OFF", { 500, 79 });
			b.onFocus = [] { gLooper.sound.Play(gLooper.res_sound_button_1); };
			b.onClicked = [&] {
				v = !v;
				b.LabelRight().SetText(v ? U"ON" : U"OFF");
			};
		}

		auto& csbar = gLooper.sliderBarCfg;
		auto& csblock = gLooper.sliderBlockCfg;
		{
			auto& b = ui->MakeChildren<xx::FocusSlider>()->Init(2, pos5 + XY{ 0, 100 }, 0.5f
				, cn, ch, csbar, csblock, 79, 250, 450, 150, U"slider", 0.5);
			b.onFocus = [] { gLooper.sound.Play(gLooper.res_sound_button_1); };
			//b.onChanged = [](double v) {};
		}
#endif
	}

	inline void MainMenu::OnWindowSizeChanged() {
		MakeUI();
	}

	inline void MainMenu::Init() {
		UpdateScale();
		MakeUI();
	}

	void MainMenu::Update() {
	}

	inline void MainMenu::Draw() {
		gLooper.DrawNode(ui);
	}

}
