#pragma once

namespace Game {

	inline void MainMenu::MakeUI() {
		ui.Emplace()->Init(0, {}, scale);

		// left bottom menu ( start, options, quit )
		{
			auto& b = ui->MakeChildren<xx::FocusButton>()->Init(2, pos1 + XY{ 30, 230 }, anchor1
				, gLooper.cfg_btnNormal, gLooper.cfg_btnHighlight, U"start");
			b.onFocus = [] { gLooper.sound.Play(gLooper.res_sound_button_1); };
			b.onClicked = [] { gLooper.DelaySwitchTo<Stage>(); };
		}
		{
			auto& b = ui->MakeChildren<xx::FocusButton>()->Init(2, pos1 + XY{ 30, 130 }, anchor1
				, gLooper.cfg_btnNormal, gLooper.cfg_btnHighlight, U"options");
			b.onFocus = [] { gLooper.sound.Play(gLooper.res_sound_button_1); };
			b.onClicked = [] { gLooper.DelaySwitchTo<Test2>(); };
		}

		{
			auto& b = ui->MakeChildren<xx::FocusButton>()->Init(2, pos1 + XY{ 30, 30 }, anchor1
				, gLooper.cfg_btnNormal, gLooper.cfg_btnHighlight, U"quit");
			b.onFocus = [] { gLooper.sound.Play(gLooper.res_sound_button_1); };
			b.onClicked = [] {  };	// todo
		}

		// right bottom icons ( language, qq? discord? )
		{
			auto& b = ui->MakeChildren<xx::FocusImageButton>()->Init(2, pos3 + XY{ -30, 30 }, anchor3
				, 64, 5, gLooper.cfg_imgBtnNormal, gLooper.cfg_imgBtnHighlight, gLooper.res.flags_cn);
			b.onFocus = [] { gLooper.sound.Play(gLooper.res_sound_button_1); };
			b.onClicked = [] { };	// todo
		}

#if 0
		auto& cn = gLooper.cfg_btnNormal;
		auto& ch = gLooper.cfg_btnHighlight;
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

		auto& csbar = gLooper.cfg_sliderBar;
		auto& csblock = gLooper.cfg_sliderBlock;
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
		gLooper.ShaderEnd();
		gLooper.res.flags_cn->tex->SetGLTexParm<GL_NEAREST>();
		xx::Quad{}.SetFrame(gLooper.res_bg_main).SetScale(lastWindowSize.y / 1080).SetColorplus(0.6f).Draw();
		gLooper.DrawNode(ui);
	}

}
