#pragma once

// todo: hit show damage number  reduce hp

namespace Game {

	inline void Test4::MakeUI() {
		ui.Emplace()->Init(0, {}, scale);
		ui->MakeChildren<xx::Button>()->Init(1, pos9 + XY{ -10, -10 }, anchor9
			, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});
		// todo
	}

	inline void Test4::OnWindowSizeChanged() {
		camera.SetBaseScale(scale);
		MakeUI();
	}

	inline void Test4::Init() {
		UpdateScale();
		MakeUI();

		pfc.Init(int32_t(100 * 1.5), int32_t(150 * 1.5), 100000);
		delta = Cfg::frameDelay;

		XY basePos{ pfc.pixelSize * 0.5f };

		camera.SetScale(scale, 10.f / 1.5);
		camera.SetOriginal(basePos);

		// fill pfc
		for (int32_t i = 0; i < 5000; ++i) {
			XY pos{ 
				gLooper.rnd.Next<float>(5, float(pfc.colsLen - 5)),
				gLooper.rnd.Next<float>(5, float(pfc.rowsLen - 15))
			};
			pfc.Add({}, pos, pos, {});
		}
	}

	XX_INLINE void Test4::Update_() {
		pfc.Update();
	}

	inline xx::RGBA8 Test4::GetRainbow() {
		auto t = colorIndex += 0.001f;
		const float r = sin(t);
		const float g = sin(t + 0.33f * 2.0f * (float)M_PI);
		const float b = sin(t + 0.66f * 2.0f * (float)M_PI);
		return { uint8_t(255 * r * r), uint8_t(255 * g * g), uint8_t(255 * b * b), 255 };
	}

	XX_INLINE void Test4::Update() {
		if (gLooper.KeyDownDelay(xx::KeyboardKeys::Escape, 0.5f)
			|| gLooper.KeyDownDelay(xx::KeyboardKeys::Q, 0.5f)) {
			gLooper.DelaySwitchTo<MainMenu>();
		}

		if (gLooper.mouse.PressedMBLeft() && pfc.datasLen < 35000) {
			static constexpr float mpRadius{ 4 };
			auto mp = camera.ToLogicPos(gLooper.mouse.pos);
			if (mp.x < mpRadius || mp.x > pfc.pixelSize.x - mpRadius
				|| mp.y < mpRadius || mp.y > pfc.pixelSize.y - mpRadius) {
			}
			else {
				for (int32_t i = 0; i < 10; ++i) {
					auto pos = mp + Shaker::GetRndPosDoughnut(gLooper.rnd, mpRadius - 1, 0);
					pfc.Add({}, pos, pos, {}, GetRainbow());
				}
			}
		}

		assert(delta > 0);
		timePool += delta;
		while (timePool >= Cfg::frameDelay) {
			timePool -= Cfg::frameDelay;
			Update_();
			++time;
		}
	}

	inline void Test4::Draw() {
		//// bg
		//{
		//	auto& t = gLooper.res_bg_fight_1;
		//	auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(t, 1);
		//	q->pos = {};
		//	q->anchor = 0.5f;
		//	q->scale = lastWindowSize.y / t->Height() * camera.scale;
		//	q->radians = 0;
		//	q->colorplus = 0.5f;
		//	q->color = xx::RGBA8_White;
		//	q->texRect = { 0, 0, (uint16_t)t->Width(), (uint16_t)t->Height() };
		//}

		// pfc
		{
#if 1
			auto len = pfc.datasLen;
			auto qs = gLooper.ShaderBegin(gLooper.shaderRingInstance).Draw(len);
			for (int32_t i = 0; i < len; ++i) {
				auto& d = pfc.datas[i];
				auto& q = qs[i];
				q.pos = camera.ToGLPos(d.pos);
				q.radius = 0.5f * camera.scale;
				q.colorPlus = 3.f;
				q.color = d.color;
			}
#else
			auto len = pfc.datasLen;
			auto& f = gLooper.res.ui_circle;
			auto qs = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(f->tex, len);
			for (int32_t i = 0; i < len; ++i) {
				auto& d = pfc.datas[i];
				auto& q = qs[i];
				q.pos = camera.ToGLPos(d.pos);
				q.anchor = 0.5f;
				q.scale = 1.f / f->spriteSize.x * camera.scale;
				q.radians = 0;
				q.colorplus = 1.f;
				q.color = xx::RGBA8_White;
				q.texRect.data = f->textureRect.data;
			}
#endif
		}

		// todo: pfc border ?

		// ui
		gLooper.DrawNode(ui);

	}
}
