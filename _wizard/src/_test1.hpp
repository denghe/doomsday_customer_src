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
		ui->MakeChildren<xx::Button>()->Init(1, pos7 + XY{ 10, -10 }
			, anchor7, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});
		ui->MakeChildren<xx::Button>()->Init(1, pos8 + XY{ 0, -10 }
			, anchor8, gLooper.btnCfg, U"add 10000 SpineBoy", [&]() {
				AddSomeSpineBoy(10000);
			});
		ui->MakeChildren<xx::Button>()->Init(1, pos8 + XY{ 0, -100 }
			, anchor8, gLooper.btnCfg, U"add 10000 FrenchFries", [&]() {
				AddSomeFrenchFries(10000);
			});
		ui->MakeChildren<xx::Button>()->Init(1, pos8 + XY{ 0, -190 }
			, anchor8, gLooper.btnCfg, U"add 10000 Slime", [&]() {
				AddSomeSlime(10000);
			});
	}




	inline void Test1::AddVertAnim(xx::Ref<xx::GLTexture> tex, xx::Ref<xx::GLVertTexture> vtex, float scale) {
		auto& va = vas.Emplace().Emplace();
		va->tex = std::move(tex);
		va->vertTex = std::move(vtex);
		va->numFrames = va->vertTex->NumFrames();
		va->pos = { gLooper.rnd.Next<float>(-800, 800), gLooper.rnd.Next<float>(-400, 400) };
		va->scale = scale;
	}

	inline void Test1::AddSomeSpineBoy(int32_t n) {
		for (int i = 0; i < n; ++i) {
			AddVertAnim(gLooper.res_SpineBoy_tex, vtSpineBoy, 0.2f);
		}
	}
	inline void Test1::AddSomeFrenchFries(int32_t n) {
		for (int i = 0; i < n; ++i) {
			AddVertAnim(gLooper.res_FrenchFries_tex, vtFrenchFries, 0.2f);
		}
	}
	inline void Test1::AddSomeSlime(int32_t n) {
		for (int i = 0; i < n; ++i) {
			AddVertAnim(gLooper.res_Slime1_tex, vtSlime);
		}
	}

	inline void Test1::Init() {
		UpdateScale();
		MakeUI();

#if 0
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

		sps.Emplace().Emplace(gLooper.res_Slime1_skel)
			->SetScale(5)
			.SetAnimation(0, gLooper.res_Slime1_idle, true);

		xx::SpinePlayer sp1{ gLooper.res_FrenchFries_skel };
		xx::SpinePlayer sp2{ gLooper.res_SpineBoy_skel };
		xx::SpinePlayer sp3{ gLooper.res_Slime1_skel };

		vtFrenchFries.Emplace(sp1.AnimToTexture(gLooper.res_FrenchFries_walk, Cfg::frameDelay));
		vtSpineBoy.Emplace(sp2.AnimToTexture(gLooper.res_SpineBoy_walk, Cfg::frameDelay));
		vtSlime.Emplace(sp3.AnimToTexture(gLooper.res_Slime1_idle, Cfg::frameDelay));
	}

	inline void Test1::Update() {
		++radians;

		for (auto& sp : sps) {
			sp->skeleton.getRootBone()->setRotation(radians);
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
			auto bone = sp->FindBone("shoot_point");
			XY pos{ bone->getWorldX(), bone->getWorldY() };
			xx::Quad().SetFrame(gLooper.res.ui_star).SetPosition(pos).Draw();
		}
		gLooper.GLBlendFunc(gLooper.blendDefault);

		gLooper.DrawNode(ui);
	}

	inline void Test1::OnWindowSizeChanged() {
		MakeUI();
	}
}
