#pragma once

namespace Game {

	inline void Stage::MakeUI() {
		ui.Emplace()->Init(0, {}, scale);
		ui->MakeChildren<xx::Button>()->Init(1, pos7 + XY{ 10, -10 }
			, anchor7, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});
	}

	inline void Stage::OnWindowSizeChanged() {
		MakeUI();
	}

	inline void Stage::Init() {
		UpdateScale();
		MakeUI();
		map.Emplace<Map>()->Init();
		camera.SetOriginal(map->blocks.gridSize / 2);
		player.Emplace()->Init(this);
		monsters.Init(&gLooper.rdd, map->blocks.numRows, map->blocks.numCols, map->blocks.cellSize);
	}

	inline void Stage::MonsterGen() {
		XX_BEGIN(_n);
		for (_a = time + int32_t(Cfg::fps * 10.f); time < _a;) {
			{
				auto idx = rnd.Next<int32_t>(map->bornPlaces_Monster.len);
				auto m = xx::MakeShared<Monster>();
				m->Init(this, gLooper.res.ui_trangle, idx);
				monsters.Add(std::move(m));
			}
			/* sleep */ for (_b = time + int32_t(Cfg::fps * 0.1f); time < _b;) { XX_YIELD(_n); }
		}
		/* sleep */ for (_b = time + int32_t(Cfg::fps * 5.f); time < _b;) { XX_YIELD(_n); }
		XX_YIELD_TO_BEGIN(_n);
		XX_END(_n);
	}

	inline void Stage::Update() {

		UpdateMap();
		UpdateEffectExplosion();
		UpdatePlayerBullet();
		UpdateMonster();
		UpdatePlayer();
		// ... more updates

		MonsterGen();

		++time;
	}

	XX_INLINE void Stage::UpdateMap() {
		map->Update();
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

	XX_INLINE void Stage::UpdateMonster() {
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
		auto areaMin = camera.ToLogicPos({ -gLooper.windowSize_2.x - Cfg::unitSize * 2, gLooper.windowSize_2.y + Cfg::unitSize * 2 }, scale);
		auto areaMax = camera.ToLogicPos({ gLooper.windowSize_2.x + Cfg::unitSize * 2, -gLooper.windowSize_2.y - Cfg::unitSize * 2 }, scale);

		// game logic content
		auto t = gLooper.fb.Draw(gLooper.windowSize, true, xx::RGBA8{ 0,0,0,0 }, [&]() {

			map->Draw(this);

			for (auto i = 0, e = monsters.items.len; i < e; ++i) {
				auto& o = monsters.items[i];
				if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
				o->Draw();
			}

			for (int32_t i = 0, e = playerBullets.len; i < e; ++i) {
				auto& o = playerBullets[i];
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
				DrawLight_Circle(camera.ToGLPos(player->pos), player->lightRadius);
			}

			for (int32_t i = 0, e = playerBullets.len; i < e; ++i) {
				auto& o = playerBullets[i];
				if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
				DrawLight_Circle(camera.ToGLPos(o->pos), o->lightRadius, 0.7f);
			}

			for (auto i = 0, e = effectExplosions.len; i < e; ++i) {
				auto& o = effectExplosions[i];
				if (o.pos.x < areaMin.x || o.pos.x > areaMax.x || o.pos.y < areaMin.y || o.pos.y > areaMax.y) continue;
				DrawLight_Circle(camera.ToGLPos(o.pos), o.lightRadius, 1.f);
			}

			for (auto i = 0, e = monsters.items.len; i < e; ++i) {
				auto& o = monsters.items[i];
				if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
				DrawLight_Circle(camera.ToGLPos(o->pos), o->lightRadius, 0.7f);
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

		//	// draw tips
		//	gLooper.ctcDefault.Draw({ 0, gLooper.windowSize_2.y - 5 }
		//		, xx::ToString("jump: SPACE  move: ASDW  m = ", monsters.items.len
		//			, " b = ", playerBullets.len
		//			, " e = ", effects.len
		//			, " et = ", effectTexts.ens.Count()
		//		), xx::RGBA8_Green, { 0.5f, 1 });
	}

}
