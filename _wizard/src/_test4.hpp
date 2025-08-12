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

		//grid.Init(32, 100, 100);
		delta = Cfg::frameDelay;

		//XY basePos{ grid.pixelSize * 0.5f };

		xx::CoutN(scale);
		camera.SetScale(scale, 0.6f);
		//camera.SetOriginal(basePos);


	}

	XX_INLINE void Test4::Update_() {

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
		// bg
		{
			auto& t = gLooper.res_bg_fight_1;
			auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(t, 1);
			q->pos = {};
			q->anchor = 0.5f;
			q->scale = lastWindowSize.y / t->Height() * camera.scale;
			q->radians = 0;
			q->colorplus = 0.5f;
			q->color = xx::RGBA8_White;
			q->texRect = { 0, 0, (uint16_t)t->Width(), (uint16_t)t->Height() };
		}

		// ui
		gLooper.DrawNode(ui);

	}

}
