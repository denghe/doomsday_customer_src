#pragma once

namespace Game {

	inline void Stage::StageInit(XYi gridSize_) {
		fb.Init();

		gridSize = gridSize_;
		mapSize = Cfg::unitSize * gridSize;

		camera.scale = Cfg::defaultScale;
		camera.mapSize = mapSize;
		camera.newOriginal = camera.original = mapSize * 0.5f;

		effectTexts.Init(this, 10000);

		ground.Emplace()->Init(this, mapSize, gLooper.res.ground_cell3);
		grasses.Reserve(100000);
		EnvGrass::GenGrass(this, 15);

		monsters.Init(&gLooper.rdd, gridSize.y, gridSize.x, (int32_t)Cfg::unitSize);
		loots.Init(&gLooper.rdd, gridSize.y, gridSize.x, (int32_t)Cfg::unitSize);
	}

	inline XY Stage::GetPlayerBornPos() {
		return mapSize * 0.5f;
	}

	inline void Stage::ForceLimit(XY& pos) {
		if (pos.x < 0.f) pos.x = 0.f;
		else if (pos.x >= mapSize.x)
			pos.x = mapSize.x - 0.1f;
		if (pos.y < 0.f) pos.y = 0.f;
		else if (pos.y >= mapSize.y)
			pos.y = mapSize.y - 0.1f;
	}

	inline bool Stage::IsOutOfMap(XY const& pos) {
		return pos.x < 0 || pos.y < 0 || pos.x >= mapSize.x || pos.y >= mapSize.y;
	}

	inline XY Stage::GetRndPosDoughnut(float maxRadius, float safeRadius) {
		auto len = maxRadius - safeRadius;
		auto len_radius = len / maxRadius;
		auto safeRadius_radius = safeRadius / maxRadius;
		auto radius = std::sqrtf(rnd.Next<float>(0, len_radius) + safeRadius_radius) * maxRadius;
		auto radians = rnd.Next<float>(-M_PI, M_PI);
		return { std::cosf(radians) * radius, std::sinf(radians) * radius };
	}

	template<bool clearPlayer, bool clearGrass>
	inline void Stage::ClearItems() {
		playerBullets.Clear();
		monsterBullets.Clear();
		if constexpr (clearPlayer) {
			player.Reset();
		}
		monsters.Clear();
		spawners.Clear();
		monsterGenerators.Clear();
		effects.Clear();
		effectTexts.Clear();
		if constexpr (clearGrass) {
			grasses.Clear();
		}
		// todo: rnd reset?
		time = 0;
	}

	template<bool updateTime>
	inline void Stage::StageUpdate() {
		//if (paused) return;

		// scale control
		if (gLooper.KeyDownDelay(xx::KeyboardKeys::Z, 0.02f)) {
			camera.IncreaseScale(0.01f, 5);
		}
		else if (gLooper.KeyDownDelay(xx::KeyboardKeys::X, 0.02f)) {
			camera.DecreaseScale(0.01f, 0.1f);
		}

		// update spawners
		for (auto i = spawners.len - 1; i >= 0; --i) {
			auto& o = spawners[i];
			if (o->Update()) {
				spawners.SwapRemoveAt(i);
			}
		}

		// update player bullets
		for (auto i = playerBullets.len - 1; i >= 0; --i) {
			auto& o = playerBullets[i];
			if (o->Update()) {
				playerBullets.SwapRemoveAt(i);
			}
		}

		// update player
		player->Update();

		// update monster bullets
		for (auto i = monsterBullets.len - 1; i >= 0; --i) {
			auto& o = monsterBullets[i];
			if (o->Update()) {
				monsterBullets.SwapRemoveAt(i);
			}
		}

		// update monsters
		for (auto i = monsters.items.len - 1; i >= 0; --i) {
			auto& o = monsters.items[i];
			if (o->Update()) {
				monsters.Remove(o);
			}
		}

		// update monster generators
		for (auto i = monsterGenerators.len - 1; i >= 0; i--) {
			auto& mg = monsterGenerators[i];
			if (mg->activeTime <= time) {
				if (mg->destroyTime >= time) {
					mg->Update();
				}
				else {
					monsterGenerators.SwapRemoveAt(i);
				}
			}
		}

		// update effects
		effectTexts.Update();
		for (auto i = effects.len - 1; i >= 0; --i) {
			auto& o = effects[i];
			if (o->Update()) {
				effects.SwapRemoveAt(i);
			}
		}
		for (auto i = grasses.len - 1; i >= 0; --i) {
			auto& o = grasses[i];
			if (o->Update()) {
				grasses.SwapRemoveAt(i);
			}
		}

		// ... more updates

		// sync cam
		camera.SetOriginal(player->pos, camera.ToLogicPos(gLooper.mouse.pos));
		camera.Update();

		// update time
		if constexpr (updateTime) {
			++time;
		}

		//// finish check
		//if (monsterGenerators.Empty() && spawners.Empty() && monsters.items.Empty()) {
		//	OnRoundEnd();
		//}
	}

	inline void Stage::Draw() {
		if (lastWindowSize != gLooper.windowSize) {
			OnWindowSizeChanged();
			lastWindowSize = gLooper.windowSize;
		}

		// calculate display cut area
		auto areaMin = camera.ToLogicPos({ -gLooper.windowSize_2.x - Cfg::unitSize * 2, gLooper.windowSize_2.y + Cfg::unitSize * 2 });
		auto areaMax = camera.ToLogicPos({ gLooper.windowSize_2.x + Cfg::unitSize * 2, -gLooper.windowSize_2.y - Cfg::unitSize * 2 });

		// game logic content
		auto t = fb.Draw(gLooper.windowSize, true, xx::RGBA8{ 0,0,0,0 }, [&]() {

			// smooth display for some game content
			gLooper.res.buff_0->tex->SetGLTexParm<GL_LINEAR>();

			// draw floor
			ground->Draw();

			// draw game items ( order by y )
			// 
			// prepare
			auto& yd = gLooper.yDraws;

			if (player) {
				yd.Emplace(player->pos.y, player.pointer);
			}

			for (auto e = playerBullets.len, i = 0; i < e; ++i) {
				auto& o = playerBullets[i];
				if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
				yd.Emplace(o->pos.y, o.pointer);
			}

			for (auto e = monsterBullets.len, i = 0; i < e; ++i) {
				auto& o = monsterBullets[i];
				if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
				yd.Emplace(o->pos.y, o.pointer);
			}

			// maybe can thread pool optimize?
			for (auto e = monsters.items.len, i = 0; i < e; ++i) {
				auto& o = monsters.items[i];
				if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
				yd.Emplace(o->pos.y, o.pointer);
			}

			for (auto e = grasses.len, i = 0; i < e; ++i) {
				auto& o = grasses[i];
				if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
				yd.Emplace(o->pos.y, o.pointer);
			}

			for (auto e = effects.len, i = 0; i < e; ++i) {
				auto& o = effects[i];
				if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
				yd.Emplace(o->pos.y, o.pointer);
			}

			DrawCustomOrderYItem(yd, areaMin, areaMax);

			// sort
			std::sort(yd.buf, yd.buf + yd.len, [](auto& a, auto& b) { return a.first < b.first; });

			// draw
			for (auto e = yd.len, i = 0; i < e; ++i) {
				yd[i].second->Draw();
			}

			// clean up
			yd.Clear();
		});

		// light tex
		auto t2 = fb.Draw(gLooper.windowSize, true, xx::RGBA8{ 0,0,0,0 }, [&] {
			gLooper.GLBlendFunc({ GL_SRC_COLOR, GL_ONE, GL_FUNC_ADD });
			if (player) {
				auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(gLooper.res._texid_ef_light, 2);
				auto pos = camera.ToGLPos(player->pos);
				q->pos = pos;
				q->anchor = 0.5f;
				q->scale = camera.scale * 15;
				q->radians = 0;
				q->colorplus = 1.f;
				q->color = xx::RGBA8_White;
				q->texRect.data = gLooper.res._uvrect_ef_light.data;
				++q;
				q->pos = pos;
				q->anchor = 0.5f;
				q->scale = camera.scale * 5;
				q->radians = 0;
				q->colorplus = 1.f;
				q->color = xx::RGBA8_Green;//xx::RGBA8_White;RGBA8_Green
				q->texRect.data = gLooper.res._uvrect_ef_light.data;
			}

			for (auto e = monsters.items.len, i = 0; i < e; ++i) {
				auto& o = monsters.items[i];
				if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
				auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(gLooper.res._texid_ef_light, 1);
				q->pos = camera.ToGLPos(o->pos);
				q->anchor = 0.5f;
				q->scale = camera.scale * 5;
				q->radians = 0;
				q->colorplus = 1.f;
				q->color = xx::RGBA8_Red;//xx::RGBA8_Red;RGBA8_White
				q->texRect.data = gLooper.res._uvrect_ef_light.data;
			}
		});

		// combine content & light
		gLooper.ShaderBegin(gLooper.shaderQuadInstanceLight).Draw(t, t2);


		// draw spawners
		for (auto e = spawners.len, i = 0; i < e; ++i) {
			auto& o = spawners[i];
			if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
			o->Draw();
		}

		// draw effect texts
		for (int32_t i = 0, e = effectTexts.ens.Count(); i < e; ++i) {
			auto& o = effectTexts.ens[i];
			if (o.pos.x < areaMin.x || o.pos.x > areaMax.x || o.pos.y < areaMin.y || o.pos.y > areaMax.y) continue;
			o.Draw(this);
		}

		// draw name
		//for (auto e = yd.len, i = 0; i < e; ++i) {
		//	yd[i].second->DrawNameBG();
		//}
		//for (auto e = yd.len, i = 0; i < e; ++i) {
		//	yd[i].second->DrawName();
		//}


		// sharp display for UI
		gLooper.ShaderEnd();
		gLooper.res.buff_0->tex->SetGLTexParm();


		// custom ui here
		DrawCustomUI();

		// draw ui
		if (ui) {
			gLooper.DrawNode(ui);
		}

		// draw tips
		gLooper.ctcDefault.Draw({ 0, gLooper.windowSize_2.y - 5 }
			, xx::ToString("zoom: ZX  move: ASDW  m = ", monsters.items.len
				, " b = ", playerBullets.len
				, " g = ", grasses.len
				, " e = ", effects.len
				, " et = ", effectTexts.ens.Count()
			), xx::RGBA8_Green, { 0.5f, 1 });
	}

}
