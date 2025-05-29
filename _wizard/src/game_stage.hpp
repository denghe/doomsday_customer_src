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
		StageInit();
	}

	inline void Stage::Update() {
		StageUpdate();
	}

	template<bool initMap>
	inline void Stage::StageInit() {
		if constexpr (initMap) {
			map.Emplace<Map>()->Init();
		}
		camera.SetOriginal(map->blocks.gridSize / 2);
		player.Emplace()->Init(this);
	}

	//template<bool clearPlayer, bool clearGrass>
	//inline void Stage::ClearItems() {
	//	playerBullets.Clear();
	//	monsterBullets.Clear();
	//	if constexpr (clearPlayer) {
	//		player.Reset();
	//	}
	//	monsters.Clear();
	//	spawners.Clear();
	//	monsterGenerators.Clear();
	//	effects.Clear();
	//	effectTexts.Clear();
	//	if constexpr (clearGrass) {
	//		ground->Clear();
	//	}
	//	// todo: rnd reset?
	//	time = 0;
	//}

	template<bool updateTime>
	inline void Stage::StageUpdate() {
		//	//if (paused) return;

			// update player bullets
		for (auto i = playerBullets.len - 1; i >= 0; --i) {
			auto& o = playerBullets[i];
			if (o->Update()) {
				playerBullets.SwapRemoveAt(i);
			}
		}

		// update player
		if (player) {
			if (player->Update()) {
				player.Reset();
			}
		}

		// update map env
		map->Update();

		//	// update monster bullets
		//	for (auto i = monsterBullets.len - 1; i >= 0; --i) {
		//		auto& o = monsterBullets[i];
		//		if (o->Update()) {
		//			monsterBullets.SwapRemoveAt(i);
		//		}
		//	}

		//	// update monsters
		//	for (auto i = monsters.items.len - 1; i >= 0; --i) {
		//		auto& o = monsters.items[i];
		//		if (o->Update()) {
		//			monsters.Remove(o);
		//		}
		//	}

		//	// update monster generators
		//	for (auto i = monsterGenerators.len - 1; i >= 0; i--) {
		//		auto& mg = monsterGenerators[i];
		//		if (mg->activeTime <= time) {
		//			if (mg->destroyTime >= time) {
		//				mg->Update();
		//			}
		//			else {
		//				monsterGenerators.SwapRemoveAt(i);
		//			}
		//		}
		//	}

		//	// update effects
		//	effectTexts.Update();
		//	for (auto i = effects.len - 1; i >= 0; --i) {
		//		auto& o = effects[i];
		//		if (o->Update()) {
		//			effects.SwapRemoveAt(i);
		//		}
		//	}

		//	// update flying loots
		//	for (auto i = flyingLoots.len - 1; i >= 0; --i) {
		//		auto& o = flyingLoots[i];
		//		if (o->Update()) {
		//			flyingLoots.SwapRemoveAt(i);
		//		}
		//	}

		//	// ... more updates

		//	// sync cam
		//	camera.SetOriginal(player->pos, camera.ToLogicPos(gLooper.mouse.pos, scale));
		//	camera.Update();

		// update time
		if constexpr (updateTime) {
			++time;
		}

		//	//// finish check
		//	//if (monsterGenerators.Empty() && spawners.Empty() && monsters.items.Empty()) {
		//	//	OnRoundEnd();
		//	//}
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

			// draw floor
			map->Draw(this);

			// todo: monster bullet ...
			for (int32_t i = 0, e = playerBullets.len; i < e; ++i) {
				auto& o = playerBullets[i];
				if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
				o->Draw();
			}


			// draw player
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

			// ...

			});

		// combine content & light
		gLooper.ShaderBegin(gLooper.shaderQuadInstanceLight).Draw(t, t2);


		//	// draw effect texts
		//	for (int32_t i = 0, e = effectTexts.ens.Count(); i < e; ++i) {
		//		auto& o = effectTexts.ens[i];
		//		if (o.pos.x < areaMin.x || o.pos.x > areaMax.x || o.pos.y < areaMin.y || o.pos.y > areaMax.y) continue;
		//		o.Draw(this);
		//	}

		//	// draw loot
		//	for (int32_t i = 0, e = loots.items.Count(); i < e; ++i) {
		//		auto& o = loots.items[i];
		//		if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
		//		o->Draw();
		//	}

		//	// draw flying loot
		//	for (int32_t i = 0, e = flyingLoots.Count(); i < e; ++i) {
		//		auto& o = flyingLoots[i];
		//		if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
		//		o->DrawFlying();
		//	}

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
