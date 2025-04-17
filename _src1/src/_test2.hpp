#pragma once

namespace Game {

	inline void Test2::Init() {
		ui.Emplace()->Init();

		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy7m + XY{ 10, -10 }
			, Cfg::xy7a, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
		});

		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy8m + XY{ 0, -10 }
			, Cfg::xy8a, gLooper.btnCfg, U"test2", [&]() {
		});

		gridSize = {60, 60};
		mapSize = 128 * gridSize;

		ground.Emplace()->Init(this, mapSize, gLooper.res.ground_cell2);
		player.Emplace<Player_1>()->Init(this);

		camera.scale = Cfg::defaultScale;
		camera.mapSize = mapSize;
		camera.newOriginal = camera.original = mapSize * 0.5f;


		s9.frame = gLooper.res.warning2;
		s9.centerOffset = { 0, -(80 - 8) / 2 };
		s9.centerAnchor = { 0, 0.5 };
		s9.centerScale = { 1, 1 };
		s9.texScale = 2;
		s9.center = {2,2,2,2};
		s9.size = { 100, 80 };
		s9.anchor = { 0, 0.5f };
	}

	void Test2::Update1() {
		XX_BEGIN(n1);
		for (s9.radians = 0.f; s9.radians < M_PI; s9.radians += 1.f * Cfg::frameDelay) {
			XX_YIELD(n1);
		}
		for (s9.radians = -M_PI; s9.radians < 0; s9.radians += 1.f * Cfg::frameDelay) {
			XX_YIELD(n1);
		}
		XX_YIELD_TO_BEGIN(n1);
		XX_END(n1);
		s9.radians += (M_PI / 2.f * Cfg::frameDelay);
	}

	void Test2::Update2() {
		XX_BEGIN(n2);
		for (s9.centerScale.y = 1.f; s9.centerScale.y >= 0; s9.centerScale.y -= 1.f * Cfg::frameDelay) {
			XX_YIELD(n2);
		}
		for (s9.centerScale.y = 0.f; s9.centerScale.y < 1.f; s9.centerScale.y += 1.f * Cfg::frameDelay) {
			XX_YIELD(n2);
		}
		XX_YIELD_TO_BEGIN(n2);
		XX_END(n2);
	}

	void Test2::Update3() {
		XX_BEGIN(n3);
		for (s9.size.x = 100; s9.size.x < 500; s9.size.x += 100.f * Cfg::frameDelay) {
			XX_YIELD(n3);
		}
		for (s9.size.x = 500; s9.size.x > 100; s9.size.x -= 100.f * Cfg::frameDelay) {
			XX_YIELD(n3);
		}
		XX_YIELD_TO_BEGIN(n3);
		XX_END(n3);
	}

	inline void Test2::Update() {
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

		Update1();
		Update2();
		Update3();

		++time;
	}

	inline void Test2::Draw() {
		ground->Draw();
		player->Draw();

		s9.pos = camera.ToGLPos(player->pos);
		s9.scale = camera.scale;
		s9.Draw();

		gLooper.DrawNode(ui);
	}
}
