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

#if 1
		for (int i = 0; i < 1; ++i) {
			auto& sp = sps.Emplace().Emplace(gLooper.res_SpineBoy_skel);
			sp->SetMix("walk", "jump", 0.2f)
				.SetMix("jump", "run", 0.2f)
				//.SetPosition(gLooper.rnd.Next<float>(-700, 700), gLooper.rnd.Next<float>(-400, 100))
				.SetPosition(0, 0)
				.AddAnimation(0, "walk", true, 0)
				.AddAnimation(0, "jump", false, 3)
				.AddAnimation(0, "run", true, 0);
		}
#endif

		auto& rnd = gLooper.rnd;


		xx::SpinePlayer sp1{ gLooper.res_FrenchFries_skel };
		xx::SpinePlayer sp2{ gLooper.res_SpineBoy_skel };

		std::array<xx::Ref<xx::GLVertTexture>, 2> vertTexs;
		vertTexs[0].Emplace(sp1.AnimToTexture(gLooper.res_FrenchFries_walk, Cfg::frameDelay));
		vertTexs[1].Emplace(sp2.AnimToTexture(gLooper.res_SpineBoy_walk, Cfg::frameDelay));

		std::array<xx::Ref<xx::GLTexture>, 2> texs;
		texs[0] = gLooper.res_FrenchFries_tex;
		texs[1] = gLooper.res_SpineBoy_tex;

		for (int i = 0; i < 10000; ++i) {
			auto idx = rnd.Next<int32_t>(0, 2);
			auto& va = vas.Emplace().Emplace();
			va->tex = texs[idx];
			va->vertTex = vertTexs[idx];
			va->numFrames = va->vertTex->NumFrames();
			va->pos = { rnd.Next<float>(-800, 800), rnd.Next<float>(-400, 400) };
			va->scale = 0.2f;
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
		for (auto& va : vas) {
			va->Draw();
		}

		for (auto& sp : sps) {
			sp->Draw();
		}
		gLooper.GLBlendFunc(gLooper.blendDefault);

		gLooper.DrawNode(ui);
	}

	inline void Test1::OnWindowSizeChanged() {
		MakeUI();
	}
}
