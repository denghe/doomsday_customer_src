#pragma once

namespace Game {

	inline void Stage1::Init() {
		ui.Emplace()->Init();
		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy1m, Cfg::xy1a, gLooper.btnCfg, U"exit", [&]() {
			gLooper.DelaySwitchTo<Game::MainMenu>();
		});

		static constexpr float imgBtnSize{ 64.f / 1080.f * Cfg::height };
		uiHUD.Emplace()->Init();
		// make pause button
		uiHUD->MakeChildren<xx::ImageButton>()->Init(1, Cfg::xy9m, Cfg::xy9a, imgBtnSize, gLooper.res.ui_pause).onClicked = [&]() {
			uiPausePanel.Popup();
		};
		// make shop button
		uiHUD->MakeChildren<xx::ImageButton>()->Init(1, Cfg::xy8m, Cfg::xy8a, imgBtnSize, gLooper.res.ui_money).onClicked = [&]() {
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
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Chips>>()->Init(this
				, 0, 18, 3, Cfg::unitRadius * 2, 1, 8);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Hamburger>>()->Init(this
				, 5, 20, 2, Cfg::unitRadius * 2, 1, 5);
		}
		break;
		case 2:
		{
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Chips>>()->Init(this
				, 0, 18, 3, Cfg::unitRadius * 2, 1, 6);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Hamburger>>()->Init(this
				, 6, 20, 2, Cfg::unitRadius * 2, 1, 5);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Hamburger, Monster_Chips>>()->Init(this
				, 10, 25, 4, 1, Cfg::unitRadius * 3, 2, 3);
		}
		break;
		case 3:
		{
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Chips>>()->Init(this
				, 0, 15, 3, Cfg::unitRadius * 2, 1, 5);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Hamburger>>()->Init(this
				, 6, 25, 2, Cfg::unitRadius * 2, 1, 6);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group3<Monster_Cola, Monster_Roastduck, Monster_Chips>>()->Init(this
				, 10, 30, 1, 1, 4,Cfg::unitRadius * 3, 2, 6);
		}
		break;
		case 4:
		{
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Chips>>()->Init(this
				, 0, 12, 3, Cfg::unitRadius * 2, 1, 5);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Roastduck, Monster_Chips>>()->Init(this
				, 8, 24, 1,3, Cfg::unitRadius * 2, 1.5, 4);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Cola>>()->Init(this
				, 18, 32, 3,  Cfg::unitRadius * 2, 1.5, 3);
		}
		break;
		case 5:
		{
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Hamburger, Monster_Chips>>()->Init(this
				, 0, 15, 4, 1, Cfg::unitRadius * 3, 2, 3);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Chips>>()->Init(this
				, 6, 20, 3, Cfg::unitRadius * 2, 1, 4);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Cola>>()->Init(this
				, 10, 25, 3, Cfg::unitRadius * 2, 1.5, 3);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Roastduck, Monster_Cola>>()->Init(this
				, 18, 35, 2, 2, Cfg::unitRadius * 3, 2, 3);
		}
		break;
		case 6:
		{
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Chips>>()->Init(this
				, 0, 14, 3, Cfg::unitRadius * 2, 1, 3);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Hamburger, Monster_Chips>>()->Init(this
				, 5, 24, 4, 1, Cfg::unitRadius * 3, 2, 5);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Cola>>()->Init(this
				, 14, 32, 3, Cfg::unitRadius * 2, 1.5, 4);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Sofa>>()->Init(this
				, 30, 38, 1, Cfg::unitRadius * 2, 2, 1);
		}
		break;
		case 7:
		{
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Chips>>()->Init(this
				, 0, 14, 15, Cfg::unitRadius * 2, 4, 4);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Roastduck, Monster_Chips>>()->Init(this
				, 6, 26, 1, 3, Cfg::unitRadius * 2, 1.5, 5);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Sofa, Monster_Chips>>()->Init(this
				, 18, 35, 1, 2, Cfg::unitRadius * 3, 1.5, 3);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Sofa>>()->Init(this
				, 34, 41, 1, Cfg::unitRadius * 2, 2, 1);
		}
		break;
		case 8:
		{
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Hamburger>>()->Init(this
				, 0, 16, 6, Cfg::unitRadius * 3, 2, 4);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Cola>>()->Init(this
				, 6, 25, 3, Cfg::unitRadius * 2, 1.5, 3);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Roastduck, Monster_Cola>>()->Init(this
				, 18, 35, 3, 2, Cfg::unitRadius * 3, 2, 4);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Laptop>>()->Init(this
				, 36, 44, 1, Cfg::unitRadius * 2, 1, 1);
		}
		break;
		case 9:
		{
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Chips>>()->Init(this
				, 0, 15, 20, Cfg::unitRadius * 4, 5, 1);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Chips>>()->Init(this
				, 6, 28, 5, Cfg::unitRadius * 3, 2, 4);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Laptop, Monster_Roastduck>>()->Init(this
				, 18, 38, 1,2, Cfg::unitRadius * 3, 2, 3);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Laptop>>()->Init(this
				, 40, 47, 1, Cfg::unitRadius * 2, 1, 1);
		}
		break;
		case 10:
		{
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Roastduck, Monster_Cola>>()->Init(this
				, 1, 16, 3, 2, Cfg::unitRadius * 3, 2, 3);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Hamburger>>()->Init(this
				, 6, 30, 6, Cfg::unitRadius * 3, 2, 4);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Sofa, Monster_Chips>>()->Init(this
				, 20, 38, 1, 2, Cfg::unitRadius * 3, 1.5, 3);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Sofa>>()->Init(this
				, 40, 47, 1, Cfg::unitRadius * 2, 2, 1);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Laptop>>()->Init(this
				, 44, 50, 1, Cfg::unitRadius * 2, 1, 1);
		}
		break;
		case 11:
		{
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Chips>>()->Init(this
				, 0, 16, 20, Cfg::unitRadius * 4, 5, 3);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Sofa, Monster_Chips>>()->Init(this
				, 5, 38, 1, 2, Cfg::unitRadius * 3, 1.5, 3);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Sofa>>()->Init(this
				, 10, 47, 1, Cfg::unitRadius * 2, 2, 3);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Laptop>>()->Init(this
				, 10, 47, 1, Cfg::unitRadius * 2, 1, 3);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Chips>>()->Init(this
				, 30, 50, 20, Cfg::unitRadius * 4, 5, 2);
		}
		break;
		case 12:
		{
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Chips>>()->Init(this
				, 0, 16, 20, Cfg::unitRadius * 4, 5, 3);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Sofa, Monster_Chips>>()->Init(this
				, 3, 38, 1, 2, Cfg::unitRadius * 3, 1.5, 5);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Sofa>>()->Init(this
				, 5, 47, 1, Cfg::unitRadius * 2, 2, 7);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Laptop>>()->Init(this
				, 5, 60, 1, Cfg::unitRadius * 2, 1, 7);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Hamburger, Monster_Chips>>()->Init(this
				, 30, 60, 4, 1, Cfg::unitRadius * 3, 2, 5);

		}
		break;
		case 13:
		{
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Cola>>()->Init(this
				, 0, 16, 3, Cfg::unitRadius * 2, 1.5, 7);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Sofa, Monster_Chips>>()->Init(this
				, 5, 38, 1, 2, Cfg::unitRadius * 3, 1.5, 1);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Roastduck>>()->Init(this
				, 10, 47, 3, Cfg::unitRadius * 2, 2, 3);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Laptop>>()->Init(this
				, 10, 50, 1, Cfg::unitRadius * 2, 1, 3);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Chips>>()->Init(this
				, 30, 50, 20, Cfg::unitRadius * 4, 5, 2);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Hamburger>>()->Init(this
				, 25, 50, 2, Cfg::unitRadius * 2, 1, 10);
		}
		break;
		case 14:
		{
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Sofa, Monster_Chips>>()->Init(this
				, 0, 16, 1, 2, Cfg::unitRadius * 3, 1.5, 5);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Cola>>()->Init(this
				, 3, 38, 3, Cfg::unitRadius * 2, 1.5, 10);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Roastduck, Monster_Cola>>()->Init(this
				, 15, 47, 3, 2, Cfg::unitRadius * 3, 2, 5);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Laptop>>()->Init(this
				, 5, 60, 1, Cfg::unitRadius * 2, 1, 20);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Hamburger, Monster_Chips>>()->Init(this
				, 30, 60, 4, 1, Cfg::unitRadius * 3, 2, 5);
		}
		break;
		case 15:
		{
			// todo house
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Cola>>()->Init(this
				, 1, 80, 3, Cfg::unitRadius * 2, 1.5, 10);
			// todo 4 hamburger 1 cola
			// todo 4 chips 1 cola
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Laptop>>()->Init(this
				, 10, 80, 1, Cfg::unitRadius * 2, 1, 15);
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Hamburger>>()->Init(this
				, 15, 80, 5, Cfg::unitRadius * 3, 2, 8);
		}
		break;
		}

	}

	inline void Stage1::Update() {
		static constexpr int32_t roundIdMax = 20;
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
		gLooper.DrawNode(uiHUD);

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
