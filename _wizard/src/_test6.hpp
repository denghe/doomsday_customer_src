#pragma once

// todo: hit show damage number  reduce hp

namespace Game {

	inline void Test6::MakeUI() {
		ui.Emplace()->Init(0, {}, scale);
		ui->MakeChildren<xx::Button>()->Init(1, pos9 + XY{ -10, -10 }, anchor9
			, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});
		// todo
	}

	inline void Test6::OnWindowSizeChanged() {
		camera.SetBaseScale(scale);
		MakeUI();
	}

	inline void Test6::FillMapTex() {
		noiseSettings.seed = 12312312312312312312L;

		auto ms = mapSize.As<int32_t>();
		auto mapData = std::make_unique_for_overwrite<xx::RGBA8[]>(ms.x * ms.y);

		auto nd = noise.GenerateHeightmap(noiseSettings, ms, false);
		for (int32_t y = 0; y < ms.y; ++y) {
			for (int32_t x = 0; x < ms.x; ++x) {
				auto i = y * ms.x + x;
				auto& md = mapData[i];
				auto v = nd[i];
				if (v > 0.3) {
					//md = xx::RGBA8_White;
					v = xx::Noise2d::Clamp01(v);
					md.r = md.g = md.b = (uint8_t)(255 * v);
					md.a = (uint8_t)255;
				}
				else {
					md = xx::RGBA8_Black;
				}
			}
		}
		mapTex = xx::LoadGLTexture<>(mapData.get(), ms.x, ms.y);
	}

	inline void Test6::Init() {
		if (!xx::Noise2d::lookup2D) xx::Noise2d::StaticInit();	// todo: move to gLooper init

		delta = Cfg::frameDelay;
		UpdateScale();
		MakeUI();

		mapSize = {192 * 1.5, 108 * 1.5};
		//noise.Init(12312312312312312312L);
		FillMapTex();

		XY basePos{ mapSize * 0.5f };

		camera.SetScale(scale, Cfg::width / mapSize.x);
		camera.SetOriginal(basePos);
	}

	XX_INLINE void Test6::Update_() {
		auto bak = noiseSettings;
		if (gLooper.KeyDown(xx::KeyboardKeys::A)) {
			noiseSettings.offset.x -= 0.1f;
		}
		if (gLooper.KeyDown(xx::KeyboardKeys::D)) {
			noiseSettings.offset.x += 0.1f;
		}
		if (gLooper.KeyDown(xx::KeyboardKeys::W)) {
			noiseSettings.offset.y -= 0.1f;
		}
		if (gLooper.KeyDown(xx::KeyboardKeys::S)) {
			noiseSettings.offset.y += 0.1f;
		}
		if (gLooper.KeyDown(xx::KeyboardKeys::Z)) {
			noiseSettings.scale -= 0.1f;
		}
		if (gLooper.KeyDown(xx::KeyboardKeys::X)) {
			noiseSettings.scale += 0.1f;
		}
		if (bak.offset != noiseSettings.offset
			|| bak.scale != noiseSettings.scale) {
			FillMapTex();
		}
	}

	XX_INLINE void Test6::Update() {
		if (gLooper.KeyDownDelay(xx::KeyboardKeys::Escape, 0.5f)
			|| gLooper.KeyDownDelay(xx::KeyboardKeys::Q, 0.5f)) {
			gLooper.DelaySwitchTo<MainMenu>();
		}

		assert(delta > 0);
		timePool += delta;
		while (timePool >= Cfg::frameDelay) {
			timePool -= Cfg::frameDelay;
			Update_();
			++time;
		}
	}

	inline void Test6::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(mapTex, 1);
		q->pos = {};
		q->anchor = 0.5f;
		q->scale = camera.scale;
		q->radians = 0;
		q->colorplus = 1.f;
		q->color = xx::RGBA8_White;
		q->texRect = { 0,0, (uint16_t)mapSize.x, (uint16_t)mapSize.y };

		// ui
		gLooper.DrawNode(ui);

	}
}
