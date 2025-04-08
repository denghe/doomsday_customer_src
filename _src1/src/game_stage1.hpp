﻿#pragma once

namespace Game {

	inline void Stage1::Init() {
		ui.Emplace()->Init();
		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy1m, Cfg::xy1a, gLooper.btnCfg, U"exit", [&]() {
			gLooper.DelaySwitchTo<Game::MainMenu>();
		});

		static constexpr float pauseButtonSize{ 64.f / 1080.f * Cfg::height };
		ui->MakeChildren<xx::ImageButton>()->Init(1, Cfg::xy9m, Cfg::xy9a, pauseButtonSize, gLooper.res.ui_pause).onClicked = [&]() {
			paused = true;	// pause
			static constexpr auto bgScale = 1080.f * 0.8f / gLooper.res._size_ui_menu.y;
			pausePanel.Emplace<xx::Image>()->Init(1, Cfg::xy5, bgScale, Cfg::xy5a, gLooper.res.ui_menu);
			pausePanel->MakeChildren<xx::Label>()->Init(2, { gLooper.res._size_ui_menu.x * 0.5f, gLooper.res._size_ui_menu.y * (4.f / 5.f) }, 1.5f, 0.5f, xx::RGBA8_Black, "Pause");
			pausePanel->MakeChildren<xx::Button>()->Init(3, { gLooper.res._size_ui_menu.x * 0.5f, gLooper.res._size_ui_menu.y * (1.f / 2.f) }, .5f, gLooper.btnCfg_Scale2, U"Restart")
				.onClicked = [this] {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			};
			pausePanel->MakeChildren<xx::Button>()->Init(3, { gLooper.res._size_ui_menu.x * 0.5f, gLooper.res._size_ui_menu.y * (1.f / 4.f) }, .5f, gLooper.btnCfg_Scale2, U"Continue")
				.onClicked = [this] {
				paused = false;	// resume
				pausePanel.Reset();
			};
		};


		uiHPBar.Init();

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
		sc->shootSpeed = 2 / Cfg::fps;
		sc->life = 3 * (int32_t)Cfg::fps;
		sc->pierceCount = 0;
		sc->pierceDelay = 0;

		{
			auto& sp = monsterGenerators.Emplace().Emplace<MonsterGen_Generic<Monster_Cola>>()
				->Init(this, 0, int32_t(Cfg::fps) * 10, 1);
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
		uiHPBar.hp = player->healthPoint;
		uiHPBar.hpMax = player->sp.healthPoint;
		uiHPBar.Draw();

		// draw pause panel ( if exists )
		if (pausePanel) {
			gLooper.DrawNode(pausePanel);
		}
	}
}
