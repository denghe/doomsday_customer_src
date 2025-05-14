#pragma once

namespace Game {

	void Test8::Update() {
		Stage::Update();

	}

	inline void Test8::Init() {
		StageInit({ 60, 60 });
		player.Emplace<Player_1>()->Init(this);

		ui.Emplace()->Init();
		MakeUI();

		camera.scale = Cfg::defaultScale;
		camera.mapSize = mapSize;
		camera.newOriginal = camera.original = mapSize * 0.5f;
	}

	inline void Test8::MakeUI() {
		ui->Clear();
		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos7 + XY{ 10, -10 }
			, gLooper.anchor7, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});

		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos8 + XY{ 0, -10 }
			, gLooper.anchor8, gLooper.btnCfg, U"Add 5000 Numbers", [this]() {
				numNumbers += 5000;
			});

		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos8 + XY{ 0, -70 }
			, gLooper.anchor8, gLooper.btnCfg, U"Add 100 Numbers", [this]() {
				numNumbers += 100;
			});
	}

	inline void Test8::OnWindowSizeChanged() {
		MakeUI();
	}



	inline void Test8::DrawCustomUI() {
		for (int i = 0; i < numNumbers; ++i) {
			auto x = rnd.Next<float>(0, gLooper.windowSize.x) - gLooper.windowSize_2.x;
			auto y = rnd.Next<float>(0, gLooper.windowSize.y) - gLooper.windowSize_2.y;
			auto q = gLooper.ShaderBegin(gLooper.shaderNumbers).Draw(1024569873);
			q->pos = { x, y };
		}
	}

}
