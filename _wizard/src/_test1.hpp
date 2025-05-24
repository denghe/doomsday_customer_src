#pragma once

namespace Game {

	inline void Test1::MakeUI() {
		ui.Emplace()->Init(0, {}, scale);
		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos7 + XY{ 10, -10 }
			, gLooper.anchor7, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});
	}

	inline void Test1::Init() {
		UpdateScale();
		MakeUI();

		sp.Emplace(gLooper.res_skelSpineBoy);
		sp->SetMix("walk", "jump", 0.2f)
			.SetMix("jump", "run", 0.2f)
			//.SetPosition(gLooper.rnd.Next<float>(-700, 700), gLooper.rnd.Next<float>(-400, 100))
			.SetPosition(0, -300)
			.AddAnimation(0, "walk", true, 0)
			.AddAnimation(0, "jump", false, 3)
			.AddAnimation(0, "run", true, 0);
	}

	inline void Test1::Update() {
		sp->Update(Cfg::frameDelay);
	}

	inline void Test1::Draw() {
		sp->Draw();
		gLooper.DrawNode(ui);
	}

	inline void Test1::OnWindowSizeChanged() {
		MakeUI();
	}
}
