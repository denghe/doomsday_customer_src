#pragma once

namespace Game {

	inline void Stage1::Init() {
		ui.Emplace()->Init();
		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy1m, Cfg::xy1a, gLooper.btnCfg, U"exit", [&]() {
			gLooper.DelaySwitchTo<Game::MainMenu>();
		});

		// todo: pause ui
		// todo: resume ui

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

		// hp bar
		static constexpr xx::UVRect hpbarCenter{ 2,2,4,4 };		// from res
		static constexpr float hpbarScale{ 1.f / 1920.f * Cfg::width };
		static constexpr float hpbarTexScale{ 4.f * hpbarScale };
		static constexpr XY hpbarSize{ 256.f * hpbarScale, 48 * hpbarScale };
		static constexpr XY hpbarPos{ Cfg::xy7.x + 10, Cfg::xy7.y - hpbarSize.y - 10 };	// left bottom
		static constexpr float hpbarContentBorderWidth{ 2 * hpbarTexScale };
		static constexpr XY hpbarContentPos{ hpbarPos.x + hpbarContentBorderWidth, hpbarPos.y + hpbarContentBorderWidth };	// left bottom
		static constexpr XY hpbarContentMaxSize{ hpbarSize - hpbarContentBorderWidth * 2 };
		static constexpr XY hpbarContentScale{ hpbarContentMaxSize / ResTpFrames::_size_ui_hpbar_content };

		// draw hp bar border
		{
			xx::Scale9 s9{
				.frame = gLooper.res.ui_hpbar,
				.center = hpbarCenter,
				.texScale{ hpbarTexScale },
				.size{ hpbarSize },
				.pos{ hpbarPos },
			};
			s9.Draw();
		}
		// draw hp bar content
		{
			XY s{ hpbarContentScale.x * ( player->healthPoint / player->sp.healthPoint ) , hpbarContentScale.y};
			auto& q = *gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(gLooper.res._texid_ui_hpbar_content, 1);
			q.pos = hpbarContentPos;
			q.anchor = {};
			q.scale = s;
			q.radians = 0;
			q.colorplus = 1;
			q.color = xx::RGBA8_White;
			q.texRect = { ResTpFrames::_uvrect_ui_hpbar_content };
		}
	}
}
