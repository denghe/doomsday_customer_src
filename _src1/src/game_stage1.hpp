#pragma once

namespace Game {

	inline void Stage1::Init() {
		ui.Emplace()->Init();
		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy7m + XY{ 10, -10 }
			, Cfg::xy7a, gLooper.btnCfg, U"exit", [&]() {
			gLooper.DelaySwitchTo<Game::MainMenu>();
		});

		gridSize = Cfg::gridSize;
		mapSize = Cfg::unitSize * gridSize;

		camera.SetMaxFrameSize(Cfg::unitSize);
		camera.SetScale(Cfg::globalScale);
		camera.SetOriginal(mapSize * 0.5f);

		ground.Emplace()->Init(this, mapSize);
		monsters.Init(&gLooper.rdd, gridSize.y, gridSize.x, (int32_t)Cfg::unitSize);

		auto& c = skillCfgs.Emplace().Emplace<SkillCfg_1>();
		c->aimRange = Cfg::unitSize * 10;
		c->radius = ResTpFrames::_size_bullet_coin5_.x * 0.5f;
		c->damage = 5;
		c->moveSpeed = 600.f / Cfg::fps;
		c->shootSpeed = 1.f / Cfg::fps;
		c->life = 3 * (int32_t)Cfg::fps;
		c->pierceCount = 0;
		c->pierceDelay = 0;

		monsterGenerators.Emplace().Emplace<MonsterGenerator_1>()
			->Init(this, 0, int32_t(Cfg::fps) * 60 * 10, 1);

		player.Emplace<Player_1>()->Init(this);
	}

	inline void Stage1::Update() {
		// scale control
		if (gLooper.KeyDownDelay(xx::KeyboardKeys::Z, 0.02f)) {
			camera.IncreaseScale(0.1f, 5);
		}
		else if (gLooper.KeyDownDelay(xx::KeyboardKeys::X, 0.02f)) {
			camera.DecreaseScale(0.1f, 0.1f);
		}

		// todo: update effects

		// update player bullets
		for (auto i = playerBullets.len - 1; i >= 0; --i) {
			auto& o = playerBullets[i];
			if (o->Update()) {
				playerBullets.SwapRemoveAt(i);
			}
		}

		// update player
		player->Update();

		// update monsters
		for (auto i = monsters.items.len - 1; i >= 0; --i) {
			auto& o = monsters.items[i];
			if (o->Update()) {
				monsters.items.SwapRemoveAt(i);
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

		// ... more updates

		// sync cam
		camera.SetOriginal(player->pos);

		// update time
		++time;
	}

	inline void Stage1::Draw() {
		// draw floor
		ground->Draw();

		// draw game items ( order by y )
		{
			// prepare
			auto& yd = gLooper.yDraws;
			yd.Emplace(player->pos.y, player.pointer);
			for (auto e = playerBullets.len, i = 0; i < e; ++i) {
				auto& o = playerBullets[i];
				yd.Emplace(o->pos.y, o.pointer);
			}
			for (auto e = monsters.items.len, i = 0; i < e; ++i) {
				auto& o = monsters.items[i];
				yd.Emplace(o->pos.y, o.pointer);
			}
			// sort
			std::sort(yd.buf, yd.buf + yd.len, [](auto& a, auto& b) {
				return a.first < b.first;
			});
			// draw
			for (auto e = yd.len, i = 0; i < e; ++i) {
				yd[i].second->Draw();
			}
			// clean up
			yd.Clear();
		}

		// draw ui
		gLooper.DrawNode(ui);

		// draw tips
		gLooper.ctcDefault.Draw({ 0, gLooper.windowSize_2.y - 5 }, "zoom: ZX  move: ASDW  ", xx::RGBA8_Green, { 0.5f, 1 });
		//gLooper.ctcDefault.Draw({ 0, gLooper.windowSize_2.y - 35 }, xx::ToString("monsters.Count() == ", monsters.Count()), xx::RGBA8_Green, { 0.5f, 1 });
	}
}
