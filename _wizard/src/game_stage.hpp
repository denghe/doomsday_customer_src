#pragma once

namespace Game {

	inline void Stage::MakeUI() {
		ui.Emplace()->Init(0, {}, scale);
		float y = -10;
		ui->MakeChildren<xx::Button>()->Init(1, pos7 + XY{ 10, y }
			, anchor7, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});
		y -= 90;
		ui->MakeChildren<xx::Button>()->Init(1, pos7 + XY{ 10, y }
			, anchor7, gLooper.btnCfg, U"GameSpeed*100", [&]() {
				frameDelay = Cfg::frameDelay * 100.f;
			});
		y -= 90;
		ui->MakeChildren<xx::Button>()->Init(1, pos7 + XY{ 10, y }
			, anchor7, gLooper.btnCfg, U"*10", [&]() {
				frameDelay = Cfg::frameDelay * 10.f;
			});
		y -= 90;
		ui->MakeChildren<xx::Button>()->Init(1, pos7 + XY{ 10, y }
			, anchor7, gLooper.btnCfg, U"*5", [&]() {
				frameDelay = Cfg::frameDelay * 5.f;
			});
		y -= 90;
		ui->MakeChildren<xx::Button>()->Init(1, pos7 + XY{ 10, y }
			, anchor7, gLooper.btnCfg, U"*1", [&]() {
				frameDelay = Cfg::frameDelay;
			});
		y -= 90;
		ui->MakeChildren<xx::Button>()->Init(1, pos7 + XY{ 10, y }
			, anchor7, gLooper.btnCfg, U"*0.7", [&]() {
				frameDelay = Cfg::frameDelay * 0.7f;
			});
		y -= 90;
		ui->MakeChildren<xx::Button>()->Init(1, pos7 + XY{ 10, y }
			, anchor7, gLooper.btnCfg, U"*0.3", [&]() {
				frameDelay = Cfg::frameDelay * 0.3f;
			});
		y -= 90;
		ui->MakeChildren<xx::Button>()->Init(1, pos7 + XY{ 10, y }
			, anchor7, gLooper.btnCfg, U"*0.1", [&]() {
				frameDelay = Cfg::frameDelay * 0.1f;
			});
		ui->MakeChildren<xx::Button>()->Init(1, pos8 + XY{ 10, -10 }
			, anchor8, gLooper.btnCfg, U"Switch Light", [&]() {
				disableLight = !disableLight;
			});
	}

	inline void Stage::OnWindowSizeChanged() {
		camera.SetBaseScale(scale);
		MakeUI();
	}

	inline void Stage::Init() {
		UpdateScale();
		MakeUI();
		map.Emplace<Map>()->Init();
		mapSize = map->blocks.gridSize;
		camera.SetOriginal(mapSize / 2);
		player.Emplace()->Init(this);
		monsterFormation.Emplace<MonsterFormation_1>()->Init(this);
		monsters.Init(&gLooper.rdd, map->blocks.numRows, map->blocks.numCols, map->blocks.cellSize);
		monsterBullets.Init(&gLooper.rdd, map->blocks.numRows, map->blocks.numCols, map->blocks.cellSize);
		frameDelay = Cfg::frameDelay;
	}

	inline void Stage::ForceLimit(XY& pos) {
		if (pos.x < 0.f) pos.x = 0.f;
		else if (pos.x >= mapSize.x)
			pos.x = mapSize.x - 0.1f;
		if (pos.y < 0.f) pos.y = 0.f;
		else if (pos.y >= mapSize.y)
			pos.y = mapSize.y - 0.1f;
	}

	XX_INLINE void Stage::MonsterFormationInit() {
		switch (roundId) {
		case 0: monsterFormation.Emplace<MonsterFormation_1>()->Init(this); break;
		// ....
		default: monsterFormation.Emplace<MonsterFormation>()->Init(this);
		}
		monsterFormation->FillShufflePoss();
	}

	inline int32_t Stage::MonsterGen() {
		XX_BEGIN(_1n);

		// batch generate monsters
		for (_1a = 0; _1a < monsterFormation->count; ++_1a) {
			{
				// make monster
				auto bornPosIdx = rnd.Next<int32_t>(map->bornPlaces_Monster.len);
				auto m = xx::MakeShared<Monster>();
				m->Init(this, bornPosIdx, monsterFormation, _1a);
				monsters.Add(std::move(m));
			}

			// sleep
			for (_1b = time + int32_t(Cfg::fps * 0.01f); time < _1b;) {
				XX_YIELD_I(_1n);
			}
		}
		XX_END(_1n);
		_1n = _1a = _1b = {};	// cleanup for next use
		return 1;
	}

	XX_INLINE bool Stage::KillRandomMonster() {
		if (auto len = monsters.items.len; !len) return true;
		else {
			auto i = rnd.Next<int32_t>(len);
			auto& o = monsters.items[i];
			gLooper.sound.Play(gLooper.res_sound_monster_die_1);
			effectExplosions.Emplace().Init(o->pos, 3.f, { 0x77,22,22,0xff });
			camera.Shake(5, 300.f * Cfg::frameDelay, int32_t(0.2f * Cfg::fps), time);
			monsters.Remove(o);
			return false;
		}
	}

	XX_INLINE bool Stage::KillAllMonster() {
		XX_BEGIN(_1n);
		while (!KillRandomMonster()) {
			for (_1b = time + int32_t(Cfg::fps * 0.01f); time < _1b;) { XX_YIELD_F(_1n); }	// sleep
			XX_YIELD_F(_1n);
		}
		XX_END(_1n);
		_1n = _1a = _1b = {};	// cleanup for next use
		return true;
	}

	XX_INLINE void Stage::Update_() {
		XX_BEGIN(_2n);
		while (true) {

			MonsterFormationInit();

			// wait all monster generate
			while (!MonsterGen()) {
				UpdateAll();
				XX_YIELD(_2n);
			}

			// wait all monster ready ( fly to target )
			while (numReadyMonsters != monsters.items.len) {
				UpdateAll();
				XX_YIELD(_2n);
			}

			// wait ? seconds for fight
			for (_2b = time + int32_t(Cfg::fps * 10.f); time < _2b;) {
				UpdateAll();
				XX_YIELD(_2n);
				if (!monsters.items.len) break;	// no monsters?
			}

			// timeout + any monster exists. if (!monsters.items.len)  escape? or move to player? path finding?

			// kill all monsters
			while (!KillAllMonster()) {
				UpdateAll();
				XX_YIELD(_2n);
			}

			// wait ? seconds for next round
			for (_2b = time + int32_t(Cfg::fps * 3.f); time < _2b;) {
				UpdateAll();
				XX_YIELD(_2n);
			}

			// next round
			++roundId;
		}
		XX_END(_2n);
	}

	inline void Stage::Update() {
		timePool += frameDelay;
		while (timePool >= Cfg::frameDelay) {
			timePool -= Cfg::frameDelay;
			Update_();
			++time;
		}
	}

	inline void Stage::UpdateAll() {
		UpdateCamera();
		UpdateMap();
		UpdateMonsterFormation();
		UpdateEffectExplosion();
		UpdatePlayerBullet();
		UpdateMonsterBullet();
		UpdateMonster();
		UpdatePlayer();
	}

	XX_INLINE void Stage::UpdateCamera() {
		camera.Update(time);
	}

	XX_INLINE void Stage::UpdateMap() {
		map->Update();
	}

	XX_INLINE void Stage::UpdateMonsterFormation() {
		monsterFormation->Update();
	}

	XX_INLINE void Stage::UpdatePlayer() {
		if (player) {
			if (player->Update()) {
				player.Reset();
			}
		}
	}

	XX_INLINE void Stage::UpdatePlayerBullet() {
		for (auto i = playerBullets.len - 1; i >= 0; --i) {
			auto& o = playerBullets[i];
			if (o->Update()) {
				playerBullets.SwapRemoveAt(i);
			}
		}
	}

	XX_INLINE void Stage::UpdateMonsterBullet() {
		for (auto i = monsterBullets.items.len - 1; i >= 0; --i) {
			auto& o = monsterBullets.items[i];
			if (o->Update()) {
				monsterBullets.Remove(o);
			}
		}
	}

	XX_INLINE void Stage::UpdateMonster() {
		numReadyMonsters = 0;
		for (auto i = monsters.items.len - 1; i >= 0; --i) {
			auto& o = monsters.items[i];
			if (o->Update()) {
				monsters.Remove(o);
			}
		}
	}

	XX_INLINE void Stage::UpdateEffectExplosion() {
		for (auto i = effectExplosions.len - 1; i >= 0; --i) {
			auto& o = effectExplosions[i];
			if (o.Update()) {
				effectExplosions.SwapRemoveAt(i);
			}
		}
	}

	inline void Stage::Draw() {
		// calculate display cut area
		auto areaMin = camera.ToLogicPos({ -gLooper.windowSize_2.x - Cfg::unitSize * 2, gLooper.windowSize_2.y + Cfg::unitSize * 2 });
		auto areaMax = camera.ToLogicPos({ gLooper.windowSize_2.x + Cfg::unitSize * 2, -gLooper.windowSize_2.y - Cfg::unitSize * 2 });

		// game logic content
		auto t = gLooper.fb.Draw(gLooper.windowSize, true, xx::RGBA8{ 0,0,0,0 }, [&]() {

			map->Draw(this);

			for (auto i = 0, e = monsters.items.len; i < e; ++i) {
				auto& o = monsters.items[i];
				if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
				o->Draw();
			}

			for (int32_t i = 0, e = monsterBullets.items.len; i < e; ++i) {
				auto& o = monsterBullets.items[i];
				if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
				o->Draw();
			}

			for (int32_t i = 0, e = playerBullets.len; i < e; ++i) {
				auto& o = playerBullets[i];
				if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
				o->Draw();
			}

			if (player) {
				player->Draw();
			}
			});

		// light tex
		auto bgColor = disableLight ? xx::RGBA8_White : xx::RGBA8{ 0,0,0,0 };
		auto t2 = gLooper.fb.Draw(gLooper.windowSize, true, bgColor, [&] {
			if (disableLight) return;

			gLooper.GLBlendFunc({ GL_SRC_COLOR, GL_ONE, GL_FUNC_ADD });
			if (player) {
				player->DrawLight();
			}

			for (int32_t i = 0, e = playerBullets.len; i < e; ++i) {
				auto& o = playerBullets[i];
				if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
				o->DrawLight();
			}

			for (auto i = 0, e = effectExplosions.len; i < e; ++i) {
				auto& o = effectExplosions[i];
				if (o.pos.x < areaMin.x || o.pos.x > areaMax.x || o.pos.y < areaMin.y || o.pos.y > areaMax.y) continue;
				o.DrawLight(this);
			}

			for (auto i = 0, e = monsters.items.len; i < e; ++i) {
				auto& o = monsters.items[i];
				if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
				o->DrawLight();
			}

			for (int32_t i = 0, e = monsterBullets.items.len; i < e; ++i) {
				auto& o = monsterBullets.items[i];
				if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
				o->DrawLight();
			}

			// ...

			});

		// combine content & light
		gLooper.ShaderBegin(gLooper.shaderQuadInstanceLight).Draw(t, t2, xx::RGBA8_White, disableLight ? 1.f : 2.f);

		for (auto i = 0, e = effectExplosions.len; i < e; ++i) {
			auto& o = effectExplosions[i];
			if (o.pos.x < areaMin.x || o.pos.x > areaMax.x || o.pos.y < areaMin.y || o.pos.y > areaMax.y) continue;
			o.Draw(this);
		}

		// draw ui
		if (ui) {
			gLooper.DrawNode(ui);
		}

		// draw tips
		gLooper.ctcDefault.Draw({ 0, gLooper.windowSize_2.y - 5 }
			, xx::ToString("jump: SPACE  move: ASDW  shoot: mouse  m = ", monsters.items.len
				, " mb = ", monsterBullets.items.len
				, " pb = ", playerBullets.len
				, " e = ", effectExplosions.len
			), xx::RGBA8_Green, { 0.5f, 1 });
	}

}
