#pragma once

namespace Game {

	inline void Test5::Init() {
		ui.Emplace()->Init();

		ui->MakeChildren<xx::Button>()->Init(10, Cfg::xy7m + XY{ 10, -10 }
			, Cfg::xy7a, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
		});

		gridSize = { 60, 60 };
		mapSize = 128 * gridSize;

		ground.Emplace()->Init(this, mapSize, gLooper.res.ground_cell2);
		player.Emplace<Player_1>()->Init(this);

		// requires: after player init. need read player's STAT
		buffInfo.Emplace()->Init();
		buffShop.Emplace()->Init(player);

		camera.scale = 1.f;
		camera.mapSize = mapSize;
		camera.newOriginal = camera.original = mapSize * 0.5f;
	}

	inline void Test5::Update() {
		// scale control
		if (gLooper.KeyDownDelay(xx::KeyboardKeys::Z, 0.02f)) {
			camera.IncreaseScale(0.01f, 5);
		}
		else if (gLooper.KeyDownDelay(xx::KeyboardKeys::X, 0.02f)) {
			camera.DecreaseScale(0.01f, 0.1f);
		}

		player->Update();
		camera.SetOriginal<true>(player->pos, camera.ToLogicPos(gLooper.mouse.pos));
		camera.Update();



		++time;
	}

	inline void Test5::Draw() {
		// draw floor
		ground->Draw();

		// calculate display cut area
		auto areaMin = camera.ToLogicPos({ -gLooper.width_2 - Cfg::unitSize * 2, gLooper.height_2 + Cfg::unitSize * 2 });
		auto areaMax = camera.ToLogicPos({ gLooper.width_2 + Cfg::unitSize * 2, -gLooper.height_2 - Cfg::unitSize * 2 });

		// draw game items ( order by y )
		// 
		// prepare
		auto& yd = gLooper.yDraws;

		yd.Emplace(player->pos.y, player.pointer);

		//for (auto e = cgs.len, i = 0; i < e; ++i) {
		//	auto& o = cgs[i];
		//	if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
		//	yd.Emplace(o->pos.y, o.pointer);
		//}

		// sort
		std::sort(yd.buf, yd.buf + yd.len, [](auto& a, auto& b) { return a.first < b.first; });

		// draw
		for (auto e = yd.len, i = 0; i < e; ++i) {
			yd[i].second->Draw();
		}

		// clean up
		yd.Clear();


		if (buffShop) {
			gLooper.DrawNode(buffShop);
		}

		if (buffShop) {
			if (auto bt = buffShop->buffBag->content->TryGetMousePosItem()) {
				buffInfo->Set(*bt, gLooper.mouse.pos);
				gLooper.DrawNode(buffInfo);
			}
		}

		gLooper.DrawNode(ui);
	}
}
