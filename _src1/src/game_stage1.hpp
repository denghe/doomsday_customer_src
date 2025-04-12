#pragma once

namespace Game {

	inline void Stage1::Init() {
		ui.Emplace()->Init();
		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy1m, Cfg::xy1a, gLooper.btnCfg, U"exit", [&]() {
			gLooper.DelaySwitchTo<Game::MainMenu>();
		});

		static constexpr float imgBtnSize{ 64.f / 1080.f * Cfg::height };
		// make pause button
		ui->MakeChildren<xx::ImageButton>()->Init(1, Cfg::xy9m, Cfg::xy9a, imgBtnSize, gLooper.res.ui_pause).onClicked = [&]() {
			uiPausePanel.Popup();
		};
		// make shop button
		ui->MakeChildren<xx::ImageButton>()->Init(1, Cfg::xy9m + XY{ -imgBtnSize - 10, 0 }, Cfg::xy9a, imgBtnSize, gLooper.res.ui_money).onClicked = [&]() {
			uiShopPanel.Popup();
		};

		uiHPBar.Init(this);
		uiPausePanel.Init(this);
		uiShopPanel.Init(this);
		// ...

		gridSize = Cfg::gridSize;
		mapSize = Cfg::unitSize * gridSize;

		camera.scale = Cfg::defaultScale;
		camera.mapSize = mapSize;
		camera.newOriginal = camera.original = mapSize * 0.5f;

		ground.Emplace()->Init(this, mapSize, gLooper.res.ground_cell2);
		monsters.Init(&gLooper.rdd, gridSize.y, gridSize.x, (int32_t)Cfg::unitSize);
		etm.Init(this, 10000);

		{
			auto& sp = monsterGenerators.Emplace().Emplace<MonsterGen_Generic<Monster_Cola>>()->Init(this, 0, int32_t(Cfg::fps) * 10, 50);
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
		player->healthPoint /= 2;	// for test

		onCleanup = [this] {
			gLooper.DelaySwitchTo<Game::Stage2>();
		};
	}

	inline void Stage1::DrawCustomUI() {
		// draw hp bar
		uiHPBar.SetValue(player->healthPoint, player->sp.healthPoint).Draw();

		// draw panels
		uiPausePanel.TryDraw();
		uiShopPanel.TryDraw();
	}
}
