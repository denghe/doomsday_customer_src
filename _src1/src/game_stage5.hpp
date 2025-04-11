#pragma once

namespace Game {

	inline void Stage5::Init() {
		ui.Emplace()->Init();
		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy1m, Cfg::xy1a, gLooper.btnCfg, U"exit", [&]() {
			gLooper.DelaySwitchTo<Game::MainMenu>();
		});

		static constexpr float imgBtnSize{ 64.f / 1080.f * Cfg::height };
		ui->MakeChildren<xx::ImageButton>()->Init(1, Cfg::xy9m, Cfg::xy9a, imgBtnSize, gLooper.res.ui_pause).onClicked = [&]() {
			uiPausePanel.Popup();
		};

		ui->MakeChildren<xx::ImageButton>()->Init(1, Cfg::xy9m + XY{ -imgBtnSize - 10, 0 }, Cfg::xy9a, imgBtnSize, gLooper.res.ui_money).onClicked = [&]() {
			// todo
		};

		uiHPBar.Init(this);
		uiPausePanel.Init(this);
		// ...

		gridSize = Cfg::gridSize;
		mapSize = Cfg::unitSize * gridSize;

		camera.scale = Cfg::defaultScale;
		camera.mapSize = mapSize;
		camera.newOriginal = camera.original = mapSize * 0.5f;

		ground.Emplace()->Init(this, mapSize, gLooper.res.ground_cell2);
		monsters.Init(&gLooper.rdd, gridSize.y, gridSize.x, (int32_t)Cfg::unitSize);
		etm.Init(this, 10000);

		//auto& sc = skillCfgs.Emplace().Emplace<SkillCfg_1>();
		//sc->aimRange = Cfg::unitSize * 20;
		//sc->radius = ResTpFrames::_size_bullet_coin5.x * 0.5f;
		//sc->damage = 5;
		//sc->moveSpeed = 800.f / Cfg::fps;
		//sc->shootSpeed = 10000 / Cfg::fps;
		//sc->life = 3 * (int32_t)Cfg::fps;
		//sc->pierceCount = 0;
		//sc->pierceDelay = 0;

		{
			auto& sp = monsterGenerators.Emplace().Emplace<MonsterGen_Generic<Monster_Roastduck>>()
				->Init(this, 0, int32_t(Cfg::fps) * 10, 2500);
			sp.healthPoint = 10;
			sp.healthRegeneration = 0;
			sp.defensePoint = 0;
			sp.dodgePoint = 0;
			sp.movementSpeedPoint = 3;
			sp.damageRatio = 1;
			sp.criticalChance = 0;
			sp.criticalBonusRatio = 0;
			sp.attackSpeed = 0.2;
			sp.luckyPoint = 0;
			sp.harvestRatio = 0;
		}

		{
			auto& sp = monsterGenerators.Emplace().Emplace<MonsterGen_Generic<Monster_Sofa>>()
				->Init(this, 0, int32_t(Cfg::fps) * 10, 2500);
			sp.healthPoint = 10;
			sp.healthRegeneration = 0;
			sp.defensePoint = 0;
			sp.dodgePoint = 0;
			sp.movementSpeedPoint = 3;
			sp.damageRatio = 1;
			sp.criticalChance = 0;
			sp.criticalBonusRatio = 0;
			sp.attackSpeed = 0.2;
			sp.luckyPoint = 0;
			sp.harvestRatio = 0;
		}

		player.Emplace<Player_2>()->Init(this);

		onCleanup = [this] {
			gLooper.DelaySwitchTo<Game::Stage1>();
		};
	}

	inline void Stage5::DrawCustomUI() {
		// draw hp bar
		uiHPBar.SetValue(player->healthPoint, player->sp.healthPoint).Draw();

		// draw pause panel
		uiPausePanel.TryDraw();
	}
}
