#pragma once

namespace Game {

	inline void Test10::MakeUI() {
		ui->Clear();
		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos7 + XY{ 10, -10 }
			, gLooper.anchor7, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});

		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos8 + XY{ -250, -10 }
			, gLooper.anchor8, gLooper.btnCfg, U"set ground 1", [this]() {
				ground.Emplace<Ground1>()->Init(this, gLooper.res.ground_cell3);
			});

		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos8 + XY{ 0, -10 }
			, gLooper.anchor8, gLooper.btnCfg, U"set ground 2", [this]() {
				ground.Emplace<Ground2>()->Init(this);
			});

		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos8 + XY{ 250, -10 }
			, gLooper.anchor8, gLooper.btnCfg, U"clear all grass", [this]() {
				ground->Clear();
			});

		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos8 + XY{ -200, -70 }
			, gLooper.anchor8, gLooper.btnCfg, U"add some grass1", [this]() {
				ground->AddSomeGrass(0.1, 1);
			});

		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos8 + XY{ 200, -70 }
			, gLooper.anchor8, gLooper.btnCfg, U"add some grass2", [this]() {
				ground->AddSomeGrass(1, 2);
			});
	}

	inline void Test10::OnWindowSizeChanged() {
		MakeUI();
	}


	inline void Test10::Init() {

		StageInit<false>({ 15, 15 });
		ground.Emplace<Ground2>()->Init(this);
		player.Emplace<Player_1>()->Init(this);
		player->movementSpeedPerFrame = 1000.f / Cfg::fps;

		ui.Emplace()->Init();
		MakeUI();
	}

	void Test10::Update() {
		Stage::Update();
	}


	inline void Test10::DrawCustomUI() {
	}

}
