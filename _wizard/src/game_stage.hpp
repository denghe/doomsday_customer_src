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
			, anchor7, gLooper.btnCfg, U"Speed*1", [&]() {
				frameDelay = Cfg::frameDelay;
			});
		y -= 90;
		ui->MakeChildren<xx::Button>()->Init(1, pos7 + XY{ 10, y }
			, anchor7, gLooper.btnCfg, U"Speed*10", [&]() {
				frameDelay = Cfg::frameDelay * 10.f;
			});
		y -= 90;
		ui->MakeChildren<xx::Button>()->Init(1, pos7 + XY{ 10, y }
			, anchor7, gLooper.btnCfg, U"Speed*20", [&]() {
				frameDelay = Cfg::frameDelay * 20.f;
			});
		y -= 90;
		ui->MakeChildren<xx::Button>()->Init(1, pos7 + XY{ 10, y }
			, anchor7, gLooper.btnCfg, U"Speed*0.2", [&]() {
				frameDelay = Cfg::frameDelay * 0.2f;
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
		monsterFormation.Emplace()->Init(this);
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

	inline void Stage::MonsterGen() {
		XX_BEGIN(_n);
		while (true) {
			// random target pos
			monsterFormation->FillShufflePoss();

			// batch generate monsters
			for (_a = 0; _a < monsterFormation->count; ++_a) {
				{
					// make monster
					auto bornPosIdx = rnd.Next<int32_t>(map->bornPlaces_Monster.len);
					auto m = xx::MakeShared<Monster>();
					m->Init(this, gLooper.res.monster_1, bornPosIdx, monsterFormation, _a);
					monsters.Add(std::move(m));
				}

				// sleep
				for (_b = time + int32_t(Cfg::fps * 0.01f); time < _b;) {
					XX_YIELD(_n);
				}
			}

			// wait all monster ready
			while (numReadyMonsters != monsters.items.len) {
				XX_YIELD(_n);
			}

			// wait ? seconds for fight
			for (_b = time + int32_t(Cfg::fps * 1.f); time < _b;) {
				// no monsters?
				if (!monsters.items.len) {
					XX_YIELD_TO_BEGIN(_n);
				}
				XX_YIELD(_n);
			}

			// if (!monsters.items.len)  escape? or move to player? path finding?
		}
		XX_END(_n);
	}

	inline void Stage::Update() {
		timePool += frameDelay;
		while (timePool >= Cfg::frameDelay) {
			timePool -= Cfg::frameDelay;

			UpdateCamera();
			UpdateMap();
			UpdateMonsterFormation();
			UpdateEffectExplosion();
			UpdatePlayerBullet();
			UpdateMonsterBullet();
			UpdateMonster();
			UpdatePlayer();
			// ... more updates

			MonsterGen();

			++time;
		}
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


	XX_INLINE void Stage::DrawLight_Circle(XY screenPos, XY radius, float colorPlus, xx::RGBA8 color) {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(gLooper.res._texid_ef_light, 1);
		q->pos = screenPos * scale;
		q->anchor = 0.5f;
		q->scale = camera.scale * radius * (1.f / (gLooper.res._size_ef_light / 2.f)) * scale;
		q->radians = 0;
		q->colorplus = colorPlus;
		q->color = color;
		q->texRect.data = gLooper.res._uvrect_ef_light.data;
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
		auto t2 = gLooper.fb.Draw(gLooper.windowSize, true, xx::RGBA8{ 11,11,11,0 }, [&] {
			gLooper.GLBlendFunc({ GL_SRC_COLOR, GL_ONE, GL_FUNC_ADD });
			if (player) {
				DrawLight_Circle(camera.ToGLPos_Logic(player->pos), player->lightRadius, 1.f, player->lightColor);
			}

			for (int32_t i = 0, e = playerBullets.len; i < e; ++i) {
				auto& o = playerBullets[i];
				if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
				DrawLight_Circle(camera.ToGLPos_Logic(o->pos), o->lightRadius, o->lightColorPlus, o->lightColor);
			}

			for (auto i = 0, e = effectExplosions.len; i < e; ++i) {
				auto& o = effectExplosions[i];
				if (o.pos.x < areaMin.x || o.pos.x > areaMax.x || o.pos.y < areaMin.y || o.pos.y > areaMax.y) continue;
				DrawLight_Circle(camera.ToGLPos_Logic(o.pos), o.lightRadius, o.lightColorPlus, o.lightColor);
			}

			for (auto i = 0, e = monsters.items.len; i < e; ++i) {
				auto& o = monsters.items[i];
				if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
				DrawLight_Circle(camera.ToGLPos_Logic(o->pos), o->lightRadius, o->lightColorPlus, o->lightColor);
			}

			for (int32_t i = 0, e = monsterBullets.items.len; i < e; ++i) {
				auto& o = monsterBullets.items[i];
				if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
				DrawLight_Circle(camera.ToGLPos_Logic(o->pos), o->lightRadius, o->lightColorPlus, o->lightColor);
			}

			// ...

			});

		// combine content & light
		gLooper.ShaderBegin(gLooper.shaderQuadInstanceLight).Draw(t, t2);

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
