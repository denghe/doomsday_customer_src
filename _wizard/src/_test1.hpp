#pragma once

namespace Game {




	inline void VertAnim::Update() {
		frameIndex += 1.f;
		if (frameIndex >= numFrames) {
			frameIndex = 0;
		}
	}

	inline void VertAnim::Draw() {
		auto v = gLooper.ShaderBegin(gLooper.shaderTexVert).Draw(tex, vertTex, 1);
		v->frameIndex = (int32_t)frameIndex;
		v->pos = pos;
		v->scale = scale;
	}








	inline void Test1::MakeUI() {
		ui.Emplace()->Init(0, {}, scale);
		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos7 + XY{ 10, -10 }
			, gLooper.anchor7, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});
	}

	inline void Test1::Init() {
		UpdateScale();
		MakeUI();

#if 0
		for (int i = 0; i < 1000; ++i) {
			auto& sp = sps.Emplace().Emplace(gLooper.res_skelSpineBoy);
			sp->SetMix("walk", "jump", 0.2f)
				.SetMix("jump", "run", 0.2f)
				.SetPosition(gLooper.rnd.Next<float>(-700, 700), gLooper.rnd.Next<float>(-400, 100))
				//.SetPosition(0, -300)
				.AddAnimation(0, "walk", true, 0)
				.AddAnimation(0, "jump", false, 3)
				.AddAnimation(0, "run", true, 0);
		}
#endif

		auto& rnd = gLooper.rnd;

		xx::Listi32<xx::Shared<xx::SpinePlayer>> sps;
		xx::Listi32<xx::Ref<xx::GLTexture>> texs;
		xx::Listi32<xx::Ref<xx::GLVertTexture>> vertTexs;

		sps.Emplace().Emplace(gLooper.res_skelFrenchFries);
		sps.Emplace().Emplace(gLooper.res_skelSpineBoy);

		texs.Emplace(gLooper.res_texFrenchFries);
		texs.Emplace(gLooper.res_texSpineBoy);

		for (auto& sp : sps) {
			vertTexs.Emplace(xx::MakeRef<xx::GLVertTexture>(sp->AnimToTexture("walk", Cfg::frameDelay)));
		}

		for (int i = 0; i < 10000; ++i) {
			auto idx = rnd.Next<int32_t>(0, sps.len);
			auto& va = vas.Emplace().Emplace();
			va->tex = texs[idx];
			va->vertTex = vertTexs[idx];
			va->numFrames = va->vertTex->NumFrames();
			va->pos = { rnd.Next<float>(-800, 800), rnd.Next<float>(-400, 400) };
		}
	}

	inline void Test1::Update() {
		for (auto& sp : sps) {
			sp->Update(Cfg::frameDelay);
		}

		for (auto& va : vas) {
			va->Update();
		}
	}

	inline void Test1::Draw() {
		for (auto& sp : sps) {
			sp->Draw();
		}
		gLooper.GLBlendFunc(gLooper.blendDefault);

		for (auto& va : vas) {
			va->Draw();
		}

		gLooper.DrawNode(ui);
	}

	inline void Test1::OnWindowSizeChanged() {
		MakeUI();
	}
}
