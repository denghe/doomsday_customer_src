#pragma once

namespace Game {

	inline void Stage2::Init() {
		ui.Emplace()->Init();
		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy7m + XY{ 10, -10 }
			, Cfg::xy7a, gLooper.btnCfg, U"exit", [&]() {
			gLooper.DelaySwitchTo<Game::MainMenu>();
		});

		gridSize = Cfg::gridSize;
		mapSize = Cfg::unitSize * gridSize;

		camera.SetMaxFrameSize(Cfg::unitSize);
		camera.SetScale(Cfg::defaultScale);
		camera.SetOriginal(mapSize * 0.5f);

		ground.Emplace()->Init(this, mapSize, gLooper.res.ground_cell2);
		monsters.Init(&gLooper.rdd, gridSize.y, gridSize.x, (int32_t)Cfg::unitSize);
		etm.Init(this, 10000);

		auto& sc = skillCfgs.Emplace().Emplace<SkillCfg_1>();
		sc->aimRange = Cfg::unitSize * 20;
		sc->radius = ResTpFrames::_size_bullet_coin5.x * 0.5f;
		sc->damage = 5;
		sc->moveSpeed = 800.f / Cfg::fps;
		sc->shootSpeed = 10 / Cfg::fps;
		sc->life = 3 * (int32_t)Cfg::fps;
		sc->pierceCount = 0;
		sc->pierceDelay = 0;

		monsterGenerators.Emplace().Emplace<MonsterGen_Generic<Monster_Laptop>>()
			->Init(this, 0, int32_t(Cfg::fps) * 10, 2.5);

		monsterGenerators.Emplace().Emplace<MonsterGen_Generic<Monster_Roastduck>>()
			->Init(this, 0, int32_t(Cfg::fps) * 10, 2.5);

		player.Emplace<Player_1>()->Init(this);

		onCleanup = [this] {
			gLooper.DelaySwitchTo<Game::Stage3>();
		};
	}

}
