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

#if 0
		for (int i = 0; i < 1000; ++i) {
			auto& sp = sps.Emplace().Emplace(gLooper.res_skelSpineBoy);
			sp->SetMix("walk", "jump", 0.2f)
				.SetMix("jump", "run", 0.2f)
				.SetPosition(gLooper.rnd.Next<float>(-700, 700), gLooper.rnd.Next<float>(-400, 100))
				//.SetPosition(0, -300)
				.AddAnimation(0, "walk", true, 0)
				.AddAnimation(0, "jump", false, 3)
				.AddAnimation(0, "run", true, 0);
		}
#endif

		for (int i = 0; i < 5000; ++i) {
			auto& sp = sps.Emplace().Emplace(gLooper.res_skelFrenchFries);
			sp->SetPosition(gLooper.rnd.Next<float>(-700, 700), gLooper.rnd.Next<float>(-400, 100))
				.AddAnimation(0, "walk", true, 0);
		}
	}

	inline void Test1::Update() {
		for (auto& sp : sps) {
			sp->Update(Cfg::frameDelay);
		}
	}

	inline void Test1::Draw() {
		for (auto& sp : sps) {
			sp->Draw();
		}
		gLooper.GLBlendFunc(gLooper.blendDefault);
		gLooper.DrawNode(ui);
	}

	inline void Test1::OnWindowSizeChanged() {
		MakeUI();
	}
}
