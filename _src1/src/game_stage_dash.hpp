#pragma once

namespace Game {

	inline void Stage_Dash::Init() {
		fb.Init();

		ui.Emplace()->Init();
		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy7m + XY{ 10, -10 }
			, Cfg::xy7a, gLooper.btnCfg, U"exit", [&]() {
			gLooper.DelaySwitchTo<Game::MainMenu>();
		});

		gridSize = Cfg::gridSize;
		mapSize = Cfg::unitSize * gridSize;

		camera.scale = Cfg::defaultScale;
		camera.mapSize = mapSize;
		camera.newOriginal = camera.original = mapSize * 0.5f;

		ground.Emplace()->Init(this, mapSize, gLooper.res.ground_cell2);
		monsters.Init(&gLooper.rdd, gridSize.y, gridSize.x, (int32_t)Cfg::unitSize);
		effectTexts.Init(this, 10000);

		//auto& sc = skillCfgs.Emplace().Emplace<SkillCfg_1>();
		//sc->aimRange = Cfg::unitSize * 20;
		//sc->radius = ResTpFrames::_size_bullet_coin5.x * 0.5f;
		//sc->damage = 5;
		//sc->moveSpeed = 800.f / Cfg::fps;
		//sc->shootSpeed = 2 / Cfg::fps;
		//sc->life = 3 * (int32_t)Cfg::fps;
		//sc->pierceCount = 0;
		//sc->pierceDelay = 0;

		//// monster dash
		//sc = skillCfgs.Emplace().Emplace<SkillCfg_1>();
		//sc->aimRange = Cfg::unitSize * 5;
		//sc->radius = ResTpFrames::_size_bullet_coin5.x * 0.5f;
		//sc->damage = 5;
		//sc->moveSpeed = 1800.f / Cfg::fps;
		//sc->shootSpeed = 0;
		//sc->life = (int32_t)Cfg::fps >> 1;
		//sc->pierceCount = 0;
		//sc->pierceDelay = 0;

		monsterGenerators.Emplace().Emplace<MonsterGen_Generic<Monster_Chips>>()
			->Init(this, 10, int32_t(Cfg::fps), 10);

		//monsterGenerators.Emplace().Emplace<MonsterGen_Generic<Monster_Cola>>()
		//	->Init(this, 0, int32_t(Cfg::fps), 1);

		//monsterGenerators.Emplace().Emplace<MonsterGenerator_Chips>()
		//	->Init(this, int32_t(Cfg::fps) * 10, int32_t(Cfg::fps) * 20, 10);
		//monsterGenerators.Emplace().Emplace<MonsterGen_Generic<Monster_Cola>>()
		//	->Init(this, 0, int32_t(Cfg::fps), 1);
		//monsterGenerators.Emplace().Emplace<MonsterGenerator_Hamburger>()
		//	->Init(this, int32_t(Cfg::fps) * 30, int32_t(Cfg::fps) * 40, 1000);
		//monsterGenerators.Emplace().Emplace<MonsterGenerator_Instantnoodles>()
		//	->Init(this, int32_t(Cfg::fps) * 40, int32_t(Cfg::fps) * 50, 10000);

		//player.Emplace<Player_1>()->Init(this);
		player.Emplace<Player_1>()->Init(this);
	}
}
