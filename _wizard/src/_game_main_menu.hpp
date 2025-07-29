#pragma once

namespace Game {

	inline void MainMenu::MakeUI() {
		/*

			[big title]

[start]
[options]
[quit]
[version]			[language][qq][discord]
		*/

		ui.Emplace()->Init(0, {}, scale);

		// big title
		ui->MakeChildren<xx::Label>()->Init(1, pos5 + XY{ 0, 200 }, 5, 0.5f, xx::RGBA8_White, UI::TextOf_title_Main());

		// left bottom menu ( start, options, quit )
		{
			auto& b = ui->MakeChildren<xx::FocusLabelButton>()->Init(2, pos1 + XY{ 30, 280 }, anchor1
				, gLooper.cfg_btnNormal, gLooper.cfg_btnHighlight, UI::TextOf_mainMenu_Start());
			b.onFocus = [] { gLooper.sound.Play(gLooper.res_sound_button_1); };
			b.onClicked = [] { gLooper.DelaySwitchTo<Stage>(); };
		}
		{
			auto& b = ui->MakeChildren<xx::FocusLabelButton>()->Init(2, pos1 + XY{ 30, 180 }, anchor1
				, gLooper.cfg_btnNormal, gLooper.cfg_btnHighlight, UI::TextOf_mainMenu_Options());
			b.onFocus = [] { gLooper.sound.Play(gLooper.res_sound_button_1); };
			b.onClicked = [] { gLooper.DelaySwitchTo<ChooseChar>(); };
		}

		{
			auto& b = ui->MakeChildren<xx::FocusLabelButton>()->Init(2, pos1 + XY{ 30, 80 }, anchor1
				, gLooper.cfg_btnNormal, gLooper.cfg_btnHighlight, UI::TextOf_mainMenu_Quit());
			b.onFocus = [] { gLooper.sound.Play(gLooper.res_sound_button_1); };
			b.onClicked = [] { gLooper.DelaySwitchTo<Test2>(); };
		}

		//  version
		ui->MakeChildren<xx::Label>()->Init(1, pos1 + XY{ 30, 30 }, 1, anchor1, xx::RGBA8_Gray, U"ver 0.1 beta");

		// right bottom icons ( language, qq? discord? )
		{
			auto flag = gLooper.res.ui_flags_[(int32_t)UI::gLanguage];
			auto& b = ui->MakeChildren<xx::FocusImageButton>()->Init(2, pos3 + XY{ -30, 30 }, anchor3
				, 64, true, 5, gLooper.cfg_imgBtnNormal, gLooper.cfg_imgBtnHighlight, std::move(flag));
			b.onFocus = [] { gLooper.sound.Play(gLooper.res_sound_button_1); };
			b.onClicked = [this] {
				UI::SetNextLanguage();
				MakeUI();
				};
		}

#if 0
		auto& cn = gLooper.cfg_btnNormal;
		auto& ch = gLooper.cfg_btnHighlight;
		{
			static bool v{};
			auto txt = v ? UI::TextOf_generic_On() : UI::TextOf_generic_Off();
			auto& b = ui->MakeChildren<xx::FocusLabelButton>()->Init(2, pos5 + XY{ 0, -200 }, 0.5f
				, cn, ch, UI::TextOf_options_blahblah(), txt, {500, 79});
			b.onFocus = [] { gLooper.sound.Play(gLooper.res_sound_button_1); };
			b.onClicked = [&] {
				v = !v;
				b.LabelRight().SetText(v ? UI::TextOf_generic_On() : UI::TextOf_generic_Off());
			};
		}

		auto& csbar = gLooper.cfg_sliderBar;
		auto& csblock = gLooper.cfg_sliderBlock;
		{
			static double v{ 0.5 };
			auto& b = ui->MakeChildren<xx::FocusSlider>()->Init(2, pos5 + XY{ 0, -300 }, 0.5f
				, cn, ch, csbar, csblock, 79, 400, 450, 150, UI::TextOf_options_volume_sound(), v);
			b.onFocus = [] { gLooper.sound.Play(gLooper.res_sound_button_1); };
			b.onChanged = [](double v_) { v = v_; };
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
		xx::Quad{}.SetFrame(gLooper.res_bg_main).SetScale(lastWindowSize.y / 1080).SetColorplus(0.6f).Draw();
		gLooper.DrawNode(ui);
	}

}
