#pragma once

// todo: hit show damage number  reduce hp

namespace Game {

	inline void Test5::MakeUI() {
		ui.Emplace()->Init(0, {}, scale);
		ui->MakeChildren<xx::Button>()->Init(1, pos9 + XY{ -10, -10 }, anchor9
			, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});
		// todo
	}

	inline void Test5::OnWindowSizeChanged() {
		camera.SetBaseScale(scale);
		MakeUI();
	}

	inline void Test5::Init() {
		UpdateScale();
		MakeUI();

		be.Init(&gLooper.rdd, 50, 75, 10000);
		delta = Cfg::frameDelay;

		XY basePos{ be.pixelSize * 0.5f };

		camera.SetScale(scale, 20.f);
		camera.SetOriginal(basePos);

		// fill be
		for (int32_t i = 0; i < 1000; ++i) {
			XY pos{ 
				gLooper.rnd.Next<float>(1, float(be.numCols- 1)),
				gLooper.rnd.Next<float>(1, float(be.numRows - 1))
			};
			auto rotation = gLooper.rnd.NextRadians<float>();
			XY velocity{
				std::cosf(rotation) * be.cMaxSpeed
				, std::sinf(rotation) * be.cMaxSpeed
			};
			be.Add({}, pos, velocity, {}/*, rotation*/, GetRainbow());
		}
	}

	XX_INLINE void Test5::Update_() {
		be.Update();

		if (gLooper.mouse.PressedMBLeft() && be.datasLen < 10000) {
			static constexpr float mpRadius{ 4 };
			auto mp = camera.ToLogicPos(gLooper.mouse.pos);
			if (mp.x < 0 || mp.x >= be.pixelSize.x || mp.y < 0 || mp.y >= be.pixelSize.y) {
			}
			else {
				for (int32_t i = 0; i < 10; ++i) {
					auto rotation = gLooper.rnd.NextRadians<float>();
					XY velocity{
						std::cosf(rotation) * be.cMaxSpeed
						, std::sinf(rotation) * be.cMaxSpeed
					};
					//velocity *= gLooper.rnd.Next<float>(2);
					be.Add({}, mp, velocity, {}/*, rotation*/, GetRainbow());
				}
			}
		}

	}

	inline xx::RGBA8 Test5::GetRainbow() {
		auto t = colorIndex += 0.1f;
		const float r = sin(t);
		const float g = sin(t + 0.33f * 2.0f * (float)M_PI);
		const float b = sin(t + 0.66f * 2.0f * (float)M_PI);
		return { uint8_t(255 * r * r), uint8_t(255 * g * g), uint8_t(255 * b * b), 255 };
	}

	XX_INLINE void Test5::Update() {
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

	inline void Test5::Draw() {
		// be
		{
#if 0
			auto len = be.datasLen;
			auto qs = gLooper.ShaderBegin(gLooper.shaderRingInstance).Draw(len);
			for (int32_t i = 0; i < len; ++i) {
				auto& d = be.datas[i];
				auto& q = qs[i];
				q.pos = camera.ToGLPos(d.pos);
				q.radius = 0.5f * camera.scale;
				q.colorPlus = 3.f;
				q.color = d.color;
			}
#else
			auto len = be.datasLen;
			auto& f = gLooper.res.fire_bullet_0;
			auto qs = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(f->tex, len);
			for (int32_t i = 0; i < len; ++i) {
				auto& d = be.datas[i];
				auto& q = qs[i];
				q.pos = camera.ToGLPos(d.pos);
				q.anchor = 0.5f;
				q.scale = 1.f / f->spriteSize.x * camera.scale;
				q.radians = std::atan2f(d.velocity.y, d.velocity.x); //d.rotation;
				q.colorplus = 1.f;
				q.color = d.color;
				q.texRect.data = f->textureRect.data;
			}
#endif
		}

		// todo: be border ?

		// ui
		gLooper.DrawNode(ui);

	}
}
