#pragma once

// todo: hit show damage number  reduce hp

namespace Game {

	inline void Test4::MakeUI() {
		ui.Emplace()->Init(0, {}, scale);
		// todo
	}

	inline void Test4::OnWindowSizeChanged() {
		camera.SetBaseScale(scale);
		MakeUI();
	}

	inline void Test4::Init() {
		UpdateScale();
		MakeUI();

		pfc.Init(200, 300, 100000);
		delta = Cfg::frameDelay;

		XY basePos{ pfc.pixelSize * 0.5f };

		camera.SetScale(scale, 5.f);
		camera.SetOriginal(basePos);

		// fill pfc
		for (int32_t i = 0; i < 50000; ++i) {
			XY pos{ 
				gLooper.rnd.Next<float>(5, float(pfc.colsLen - 5)),
				gLooper.rnd.Next<float>(5, float(pfc.rowsLen - 5))
			};
			pfc.Add({}, pos, pos, {});
		}
	}

	XX_INLINE void Test4::Update_() {
		pfc.Update();
	}

	XX_INLINE void Test4::Update() {
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
		}

		// todo: pfc border ?

		// ui
		gLooper.DrawNode(ui);

	}
}
