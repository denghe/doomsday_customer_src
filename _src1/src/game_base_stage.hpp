#pragma once

namespace Game {

	inline XY Stage::GetPlayerBornPos() {
		return mapSize * 0.5f;
	}

	inline void Stage::ForceLimit(XY& pos) {
		if (pos.x < 0.f) pos.x = 0.f;
		else if (pos.x >= mapSize.x)
			pos.x = mapSize.x - std::numeric_limits<float>::epsilon();
		if (pos.y < 0.f) pos.y = 0.f;
		else if (pos.y >= mapSize.y)
			pos.y = mapSize.y - std::numeric_limits<float>::epsilon();
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


	inline void Stage::Update() {
		if (paused) return;

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
		etm.Update();
		for (auto i = effects.len - 1; i >= 0; --i) {
			auto& o = effects[i];
			if (o->Update()) {
				effects.SwapRemoveAt(i);
			}
		}

		// ... more updates

		// sync cam
		camera.SetOriginal(player->pos);

		// update time
		++time;

		// finish check
		if (monsterGenerators.Empty() && spawners.Empty() && monsters.items.Empty()) {
			onCleanup();
		}
	}

	inline void Stage::Draw() {
		// draw floor
		ground->Draw();

		// draw spawners
		for (auto e = spawners.len, i = 0; i < e; ++i) {
			spawners[i]->Draw();
		}

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
			for (auto e = effects.len, i = 0; i < e; ++i) {
				auto& o = effects[i];
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

		etm.Draw();

		// custom ui here
		DrawCustomUI();

		// draw ui
		gLooper.DrawNode(ui);

		// draw tips
		gLooper.ctcDefault.Draw({ 0, gLooper.windowSize_2.y - 5 }
			, xx::ToString("zoom: ZX  move: ASDW  m = ", monsters.items.len
				, " b = ", playerBullets.len
				, " e = ", etm.ens.Count()), xx::RGBA8_Green, { 0.5f, 1 });
	}

}
