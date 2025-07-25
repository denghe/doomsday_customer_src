#pragma once

namespace Game {

	inline void MainMenu::MakeUI() {
		ui.Emplace()->Init(0, {}, scale);

		ui->MakeChildren<xx::Button>()->Init(2, pos5 + XY{ 0, 0 }, anchor5, gLooper.btnCfg, U"new game").onClicked = [&]() {
			gLooper.DelaySwitchTo<Stage>(); 
			};
		ui->MakeChildren<xx::Button>()->Init(2, pos5 + XY{ 0, -100 }, anchor5, gLooper.btnCfgSmall, U"test2").onClicked = [&]() {
			gLooper.DelaySwitchTo<Test2>(); 
			};

		auto& cn = gLooper.btnCfgNormal;
		auto& ch = gLooper.btnCfgHighlight;
		auto& csbar = gLooper.sliderBarCfg;
		auto& csblock = gLooper.sliderBlockCfg;
		{
			auto& b = ui->MakeChildren<xx::FocusButton>()->Init(2, pos5 + XY{ -100, 300 }, 0.5f
				, cn, ch, U"btn1");
			b.onFocus = [] { gLooper.sound.Play(gLooper.res_sound_button_1); };
			b.onClicked = [] {};
		}
		{
			auto& b = ui->MakeChildren<xx::FocusButton>()->Init(2, pos5 + XY{ 100, 300 }, 0.5f
				, cn, ch, U"btn2");
			b.onFocus = [] { gLooper.sound.Play(gLooper.res_sound_button_1); };
			b.onClicked = [] {};
		}
		{
			auto& b = ui->MakeChildren<xx::FocusButton>()->Init(2, pos5 + XY{ 300, 300 }, 0.5f
				, cn, ch, U"btn3");
			b.onFocus = [] { gLooper.sound.Play(gLooper.res_sound_button_1); };
			b.onClicked = [] {};
		}
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
		{
			auto& b = ui->MakeChildren<xx::FocusSlider>()->InitBegin(2, pos5 + XY{ 0, 100 }, 0.5f
				, cn, ch, csbar, csblock, 79, 250, 450, 150, U"slider");
			//b.valueToString = [](int32_t v)->std::u32string {
			//	// todo
			//};
			b.value = 0.5f;
			b.InitEnd();
			b.onFocus = [] { gLooper.sound.Play(gLooper.res_sound_button_1); };
		}
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
