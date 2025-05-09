#pragma once

namespace Game {

	inline void Stage1::Init() {
		fb.Init();

		ui.Emplace()->Init();
		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy1m, Cfg::xy1a, gLooper.btnCfg, U"exit", [&]() {
			gLooper.DelaySwitchTo<Game::MainMenu>();
			});

		static constexpr float imgBtnSize{ 64.f / 1080.f * Cfg::height };
		uiHUD.Emplace()->Init();
		// make pause button
		uiHUD->MakeChildren<xx::ImageButton>()->Init(1, Cfg::xy9m, Cfg::xy9a, imgBtnSize, gLooper.res.ui_pause).onClicked = [&]() {
			uiPausePanel.Init(player);
			};
		// make shop button
		uiHUD->MakeChildren<xx::ImageButton>()->Init(1, Cfg::xy8m, Cfg::xy8a, imgBtnSize, gLooper.res.ui_money).onClicked = [&]() {
			uiShopPanel.Init(player);
			};

		uiHPBar.Init(this);
		uiCoinBar.Init(this);
		// ...

		gridSize = { 30, 20 };
		mapSize = Cfg::unitSize * gridSize;

		camera.scale = Cfg::defaultScale;
		camera.mapSize = mapSize;
		camera.newOriginal = camera.original = mapSize * 0.5f;

		effectTexts.Init(this, 10000);

		ground.Emplace()->Init(this, mapSize, gLooper.res.ground_cell3);
		grasses.Reserve(100000);
		EnvGrass::GenGrass(this, 15);

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

		auto gen2Hamburger = [this](float activeSeconds_, float destroySeconds, int32_t numGroups) {
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Hamburger>>()->Init(this
				, activeSeconds_, destroySeconds, 2, Cfg::unitRadius * 2, 1, numGroups);
			};
		auto gen3Chips = [this](float activeSeconds_, float destroySeconds, int32_t numGroups) {
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Chips>>()->Init(this
				, activeSeconds_, destroySeconds, 3, Cfg::unitRadius * 2, 1, numGroups);
			};
		auto gen4Hamburger1Chips = [this](float activeSeconds_, float destroySeconds, int32_t numGroups) {
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Hamburger, Monster_Chips>>()->Init(this
				, activeSeconds_, destroySeconds, 4, 1, Cfg::unitRadius * 3, 2, numGroups);
			};
		auto gen1RoastDuck1Cola4Chips = [this](float activeSeconds_, float destroySeconds, int32_t numGroups) {
			monsterGenerators.Emplace().Emplace<MonsterGen_Group3<Monster_Roastduck, Monster_Cola, Monster_Chips>>()->Init(this
				, activeSeconds_, destroySeconds, 1, 1, 4, Cfg::unitRadius * 3, 2, numGroups);
			};
		auto gen3Cola = [this](float activeSeconds_, float destroySeconds, int32_t numGroups) {
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Cola>>()->Init(this
				, activeSeconds_, destroySeconds, 3, Cfg::unitRadius * 2, 1.5, numGroups);
			};
		auto gen15Chips = [this](float activeSeconds_, float destroySeconds, int32_t numGroups) {
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Chips>>()->Init(this
				, activeSeconds_, destroySeconds, 15, Cfg::unitRadius * 4, 4, numGroups);
			};
		auto gen1RoastDuck3Chips = [this](float activeSeconds_, float destroySeconds, int32_t numGroups) {
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Roastduck, Monster_Chips>>()->Init(this
				, activeSeconds_, destroySeconds, 1,3, Cfg::unitRadius * 2, 1.5, numGroups);
			};
		auto gen3RoastDuck = [this](float activeSeconds_, float destroySeconds, int32_t numGroups) {
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Roastduck>>()->Init(this
				, activeSeconds_, destroySeconds, 3, Cfg::unitRadius * 2, 2, numGroups);
			};
		auto gen5Hamburger = [this](float activeSeconds_, float destroySeconds, int32_t numGroups) {
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Hamburger>>()->Init(this
				, activeSeconds_, destroySeconds, 5, Cfg::unitRadius * 3, 2, numGroups);
			};
		auto gen2RoastDuck2Cola = [this](float activeSeconds_, float destroySeconds, int32_t numGroups) {
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Roastduck, Monster_Cola>>()->Init(this
				, activeSeconds_, destroySeconds, 2, 2, Cfg::unitRadius * 3, 2, numGroups);
			};
		auto gen1Sofa = [this](float activeSeconds_, float destroySeconds, int32_t numGroups) {
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Sofa>>()->Init(this
				, activeSeconds_, destroySeconds, 1, Cfg::unitRadius * 2, 1, numGroups);
			};
		auto gen1Sofa2Chips = [this](float activeSeconds_, float destroySeconds, int32_t numGroups) {
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Sofa, Monster_Chips>>()->Init(this
				, activeSeconds_, destroySeconds, 1, 2, Cfg::unitRadius * 3, 1.5, numGroups);
			};
		auto gen6Hamburger = [this](float activeSeconds_, float destroySeconds, int32_t numGroups) {
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Hamburger>>()->Init(this
				, activeSeconds_, destroySeconds, 6, Cfg::unitRadius * 3, 2, numGroups);
			};
		auto gen5Cola = [this](float activeSeconds_, float destroySeconds, int32_t numGroups) {
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Cola>>()->Init(this
				, activeSeconds_, destroySeconds, 5, Cfg::unitRadius * 3, 2, numGroups);
			};
		auto gen20Chips = [this](float activeSeconds_, float destroySeconds, int32_t numGroups) {
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Chips>>()->Init(this
				, activeSeconds_, destroySeconds, 20, Cfg::unitRadius * 4, 5, numGroups);
			};
		auto gen3RoastDuck2Cola = [this](float activeSeconds_, float destroySeconds, int32_t numGroups) {
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Roastduck, Monster_Cola>>()->Init(this
				, activeSeconds_, destroySeconds, 3, 2, Cfg::unitRadius * 3, 2, numGroups);
			};
		auto gen1Laptop = [this](float activeSeconds_, float destroySeconds, int32_t numGroups) {
			monsterGenerators.Emplace().Emplace<MonsterGen_Group<Monster_Laptop>>()->Init(this
				, activeSeconds_, destroySeconds, 1, Cfg::unitRadius * 2, 1, numGroups);
			};
		auto gen1Laptop2RoastDuck = [this](float activeSeconds_, float destroySeconds, int32_t numGroups) {
			monsterGenerators.Emplace().Emplace<MonsterGen_Group2<Monster_Laptop, Monster_Roastduck>>()->Init(this
				, activeSeconds_, destroySeconds, 1, 2, Cfg::unitRadius * 3, 2, numGroups);
			};

		// fill round monster generator
		switch (roundId) {
		case 1:
		{
			gen3Chips(0, 18, 8);
			gen2Hamburger(5, 20, 5);
		}
		break;
		case 2:
		{
			gen3Chips(0, 18, 6);
			gen2Hamburger(6, 20, 5);
			gen4Hamburger1Chips(10, 25, 3);
		}
		break;
		case 3:
		{
			gen3Chips(0, 15, 5);
			gen2Hamburger(6, 25, 6);
			gen1RoastDuck1Cola4Chips(10, 30, 4);
		}
		break;
		case 4:
		{
			gen15Chips(0, 12, 2);
			gen1RoastDuck3Chips(8,24,4);
			gen3Cola(18, 32, 3);
		}
		break;
		case 5:
		{
			gen4Hamburger1Chips(0, 15, 3);
			gen3Chips(6, 20, 4);
			gen3Cola(10, 25, 3);
			gen2RoastDuck2Cola(18, 35, 3);
		}
		break;
		case 6:
		{
			gen3Chips(0, 14, 6);
			gen4Hamburger1Chips(5, 24, 5);
			gen3Cola(14, 32, 4);
			gen1Sofa(30, 38, 1);
		}
		break;
		case 7:
		{
			gen15Chips(0, 16, 2);
			gen1RoastDuck3Chips(6, 26, 5);
			gen1Sofa2Chips(18, 35, 3);
			gen3Cola(34, 41, 1);
		}
		break;
		case 8:
		{
			gen6Hamburger(0, 16, 4);
			gen3Cola(6, 25, 5);
			gen3RoastDuck2Cola(18, 35, 4);
			gen1Laptop(36, 44, 1);
		}
		break;
		case 9:
		{
			gen20Chips(0, 15, 1);
			gen5Cola(6, 28, 4);
			gen1Laptop2RoastDuck(18, 38, 3);
			gen1Laptop(40, 47, 1);
		}
		break;
		case 10:
		{
			gen3RoastDuck2Cola(0, 16, 3);
			gen6Hamburger(6, 30, 4);
			gen1Sofa2Chips(20, 38, 3);
			gen1Sofa(40, 47, 1);
			gen1Laptop(44, 50, 1);
		}
		break;
		case 11:
		{
			gen20Chips(0, 16, 3);
			gen1Sofa2Chips(5, 38, 3);
			gen1Sofa(10, 47, 3);
			gen1Laptop(10, 50, 3);
			gen20Chips(30, 50, 2);
		}
		break;
		case 12:
		{
			gen20Chips(0, 16, 3);
			gen1Sofa2Chips(3, 38, 5);
			gen1Sofa(5, 47, 7);
			gen1Laptop(5, 60, 7);
			gen4Hamburger1Chips(30, 60, 5);
		}
		break;
		case 13:
		{
			gen3Cola(0, 16, 7);
			gen1Sofa2Chips(5, 38, 1);
			gen3RoastDuck(10, 47, 3);
			gen1Laptop(10, 50, 3);
			gen20Chips(30, 50, 2);
			gen2Hamburger(25, 50, 10);
		}
		break;
		case 14:
		{
			gen1Sofa2Chips(0, 16, 5);
			gen3Cola(3, 38, 10);
			gen3RoastDuck2Cola(15, 47, 5);
			gen1Laptop(5, 60, 20);
			gen4Hamburger1Chips(30, 60, 5);
		}
		break;
		case 15:
		{
			// todo house
			gen3Cola(1, 80, 10);
			// todo 4 hamburger 1 cola
			// todo sofa 2 chips
			gen1Laptop(10, 80, 15);
			gen5Hamburger(15, 80, 8);
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
				if (!uiPausePanel) {
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
			ClearItems<false, false>();

			// handle round end event
			player->OnRoundEnd();

			// popup shop?
			if (roundId < roundIdMax) {
				uiShopPanel.Init(player);
				while (uiShopPanel) {
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
