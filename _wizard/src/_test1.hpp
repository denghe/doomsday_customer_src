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
	}

	inline void Test1::Update() {
	}

	inline void Test1::Draw() {
		gLooper.DrawNode(ui);
	}

	inline void Test1::OnWindowSizeChanged() {
		MakeUI();
	}
}
