﻿#pragma once

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
		player.Emplace()->Init(this);
		monsters.Init(&gLooper.rdd, gridSize.y, gridSize.x, (int32_t)Cfg::unitSize);
	}

	inline void Stage1::Update() {
		// scale control
		if (gLooper.KeyDownDelay(xx::KeyboardKeys::Z, 0.02f)) {
			camera.IncreaseScale(0.1f, 5);
		}
		else if (gLooper.KeyDownDelay(xx::KeyboardKeys::X, 0.02f)) {
			camera.DecreaseScale(0.1f, 0.1f);
		}

		for (auto i = playerBullets.len - 1; i >= 0; --i) {
			auto& o = playerBullets[i];
			if (o->Update()) {
				playerBullets.SwapRemoveAt(i);
			}
		}

		player->Update();

		for (auto i = monsters.items.len - 1; i >= 0; --i) {
			auto& o = monsters.items[i];
			if (o->Update()) {
				monsters.items.SwapRemoveAt(i);
			}
		}

		// ... more updates

		camera.SetOriginal(player->pos);
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
				yd.Emplace(o->pos.x, o.pointer);
			}
			for (auto e = monsters.items.len, i = 0; i < e; ++i) {
				auto& o = monsters.items[i];
				yd.Emplace(o->pos.x, o.pointer);
			}
			// sort
			std::sort(yd.buf, yd.buf + yd.len, [](auto a, auto b) {
				return std::get<0>(a) < std::get<0>(b);
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
