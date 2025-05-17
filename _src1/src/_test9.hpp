#pragma once

namespace Game {

	void Test9::Update() {
		Stage::Update();

	}

	inline void Test9::Init() {
		StageInit({ 60, 60 });
		player.Emplace<Player_1>()->Init(this);

		ui.Emplace()->Init();
		MakeUI();
	}

	inline void Test9::MakeUI() {
		ui->Clear();
		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos7 + XY{ 10, -10 }
			, gLooper.anchor7, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});

		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos8 + XY{ 0, -10 }
			, gLooper.anchor8, gLooper.btnCfg, U"Add 5000 HPBars", [this]() {
				numHPBars += 5000;
			});

		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos8 + XY{ 0, -70 }
			, gLooper.anchor8, gLooper.btnCfg, U"Add 100000 HPBars", [this]() {
				numHPBars += 100000;
			});
	}

	inline void Test9::OnWindowSizeChanged() {
		MakeUI();
	}



	inline void Test9::DrawCustomUI() {
		for (int i = 0; i < numHPBars; ++i) {
			auto x = rnd.Next<float>(0, gLooper.windowSize.x) - gLooper.windowSize_2.x;
			auto y = rnd.Next<float>(0, gLooper.windowSize.y) - gLooper.windowSize_2.y;
			auto v = rnd.Next<int32_t>(0, 10000 + 1);
			auto q = gLooper.ShaderBegin(gLooper.shaderHPBar).Draw(v, 10000);
			q->pos = { x, y };
		}
		auto q = gLooper.ShaderBegin(gLooper.shaderHPBar).Draw(7, 20);
		q->pos = {-400, 0};
		q->scale = 10;
	}

}
