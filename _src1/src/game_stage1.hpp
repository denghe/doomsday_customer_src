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
		ui->MakeChildren<xx::ImageButton>()->Init(1, Cfg::xy8m, Cfg::xy8a, imgBtnSize, gLooper.res.ui_money).onClicked = [&]() {
			uiShopPanel.Popup();
		};

		uiHPBar.Init(this);
		uiCoinBar.Init(this);
		uiPausePanel.Init(this);
		uiShopPanel.Init(this);
		// ...

		gridSize = {30, 20};
		mapSize = Cfg::unitSize * gridSize;

		camera.scale = Cfg::defaultScale;
		camera.mapSize = mapSize;
		camera.newOriginal = camera.original = mapSize * 0.5f;

		effectTexts.Init(this, 10000);

		// maybe these init can move to RoundInit()
		ground.Emplace()->Init(this, mapSize, gLooper.res.ground_cell2);
		monsters.Init(&gLooper.rdd, gridSize.y, gridSize.x, (int32_t)Cfg::unitSize);

	}

	inline void Stage1::RoundInit() {
		// reset camera
		camera.newOriginal = camera.original = mapSize * 0.5f;

		// rebuild player
		player.Emplace<Player_2>()->Init(this);

		// restore player some data ( inherit from last round )
		player->coin = coinBak;
		player->buffs = buffsBak;
		player->buffs.owner = player.ToWeak();

		// call round begin event handler
		player->OnRoundBegin();

		// fill round monster generator
		switch (roundId) {
		case 1:
		{
			auto& sp = monsterGenerators.Emplace().Emplace<MonsterGen_Generic<Monster_Cola>>()->Init(this, 0, int32_t(Cfg::fps) * 2, 1);
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
		break;
		case 2:
		{
			auto& sp = monsterGenerators.Emplace().Emplace<MonsterGen_Generic<Monster_Chips>>()->Init(this, 0, int32_t(Cfg::fps) * 2, 1);
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
			break;
		case 3:
		{
		}
			break;
			// ...
		}
	}

	inline void Stage1::Update() {
		static constexpr int32_t roundIdMax = 2;
		XX_BEGIN(n);
		/********************************************************************/
		// todo: more init here

		for (roundId = 1; roundId <= roundIdMax; ++roundId) {

			/********************************************************************/
			RoundInit();

			/********************************************************************/
			// show Round xxxx ? secs
			{
				ui1.Emplace<xx::Label>()->Init(1, { 0, 300 }, 6, 0.5f, xx::RGBA8_Red, xx::ToString("round ", roundId));
				for (sleepCounter = gLooper.frameNumber + int32_t(Cfg::fps * 1.5f); sleepCounter > gLooper.frameNumber;) {
					XX_YIELD(n);
				}
				ui1.Reset();
			}

			/********************************************************************/
			// battle
			// todo: 60 seconds timeout check ( finish game condition 2 )

			while (!(monsterGenerators.Empty() && spawners.Empty() && monsters.items.Empty())) {
				// maybe button pause game
				if (!paused) {
					UpdateItems();
				}
				XX_YIELD(n);
			}

			/********************************************************************/
			// wait 2 seconds
			// todo: absorb all stuff?
			for (sleepCounter = gLooper.frameNumber + int32_t(Cfg::fps * 2.f); sleepCounter > gLooper.frameNumber;) {
				UpdateItems();
				XX_YIELD(n);
			}

			/********************************************************************/
			// show info ? secs
			{
				ui1.Emplace<xx::Label>()->Init(1, { 0, 300 }, 6, 0.5f, xx::RGBA8_Red, "round finish!");
				for (sleepCounter = gLooper.frameNumber + int32_t(Cfg::fps * 1.5f); sleepCounter > gLooper.frameNumber;) {
					XX_YIELD(n);
				}
				ui1.Reset();
			}

			// cleanup without player
			ClearItems<false>();

			// handle round end event
			player->OnRoundEnd();

			// popup shop?
			if (roundId < roundIdMax) {
				uiShopPanel.Popup();
				while (paused) {
					XX_YIELD(n);
				}
			}

			// backup & cleanup player
			coinBak = player->coin;
			buffsBak = player->buffs;
			player.Reset();
		}

		/********************************************************************/
		// show info ? secs
		{
			ui1.Emplace<xx::Label>()->Init(1, { 0, 200 }, 5, 0.5f, xx::RGBA8_Red, "congratulations!!!");
			for (sleepCounter = gLooper.frameNumber + int32_t(Cfg::fps * 3.f); sleepCounter > gLooper.frameNumber;) {
				XX_YIELD(n);
			}
			ui1.Reset();
		}

		// return menu
		gLooper.DelaySwitchTo<Game::MainMenu>();
		XX_END(n);
	}

	inline void Stage1::DrawCustomUI() {
		// draw game uis
		if (ui1) gLooper.DrawNode(ui1);
		if (ui2) gLooper.DrawNode(ui2);
		if (ui3) gLooper.DrawNode(ui3);

		// draw hp bar
		if (player) {
			uiHPBar.SetValue(player->healthPoint, player->sp.healthPoint).Draw();
			uiCoinBar.SetValue(player->coin).Draw();
		}

		// draw panels
		uiPausePanel.TryDraw();
		uiShopPanel.TryDraw();
	}
}
