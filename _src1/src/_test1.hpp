#pragma once

namespace Game {

	inline void Test1::MakeUI() {
		ui->Clear();
		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos7 + XY{ 10, -10 }
			, gLooper.anchor7, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});

		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos8 + XY{ 0, -60 }
			, gLooper.anchor8, gLooper.btnCfg, U"Add More Monsters", [this]() {

				for (int i = 0; i < 100; i++) {
					auto x = rnd.Next<float>(0, mapSize.x);
					auto y = rnd.Next<float>(0, mapSize.y);
					spawners.Emplace().Emplace()->Init(this, { x, y }, 1.f, 1.5f, [](Stage* stage_, XY const& pos_) {
						auto m = xx::MakeShared<Monster_Chips>();
						m->Init(stage_, pos_);
						stage_->monsters.Add(std::move(m));
						});
				}
			});
	}

	inline void Test1::OnWindowSizeChanged() {
		MakeUI();
	}

	inline void Test1::Init() {
		StageInit({30, 20});
		player.Emplace<Player_1>()->Init(this);

		ui.Emplace()->Init();
		MakeUI();
	}

}
