#pragma once

namespace Game {

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

		auto sp = xx::MakeShared<xx::SpinePlayer>(
			//gLooper.res_skelFrenchFries);
			gLooper.res_skelSpineBoy);
		sp->SetPosition(0, 0).AddAnimation(0, "walk", true, 0);
		gLooper.ShaderEnd();
		sp->Draw();	// draw once for get vert size
		auto&& shader = gLooper.ShaderBegin(gLooper.shaderSpine38);
		auto vc = shader.vertsCount;

		auto rowByteSize = vc * 32;			// sizeof(float) * 8
		auto texWidth = rowByteSize / 16;	// sizeof(RGBA32F)
		texWidth = (texWidth + 7) & ~7u;
		rowByteSize = texWidth * 16;

		numFrames = 64;						// todo: calc anim length
		auto texHeight = numFrames;
		texHeight = (texHeight + 7) & ~7u;

		auto d = std::make_unique<char[]>(rowByteSize * texHeight);
		auto vs = shader.verts.get();
		for (int i = 0; i < numFrames; ++i) {
			auto bp = (xx::TexData*)(d.get() + rowByteSize * i);
			for (int j = 0; j < vc; ++j) {
				auto& p = bp[j];
				auto& v = vs[j];
				p.x = v.pos.x;
				p.y = v.pos.y;
				p.u = v.uv.x;
				p.v = v.uv.y;
				p.r = v.color.r * 0.003921568627451f;
				p.g = v.color.g * 0.003921568627451f;
				p.b = v.color.b * 0.003921568627451f;
				p.a = v.color.a * 0.003921568627451f;
			}
			shader.vertsCount = 0;
			shader.lastTextureId = 0;
			sp->Update(Cfg::frameDelay);
			sp->Draw();
		}
		shader.vertsCount = 0;
		shader.lastTextureId = 0;
		tex.Emplace(xx::LoadGLTexture_RGBA32F(d.get(), texWidth, texHeight, vc));

		xx::CoutN("vertsCount = ", vc);
	}

	inline void Test1::Update() {
		for (auto& sp : sps) {
			sp->Update(Cfg::frameDelay);
		}
		++frameIndex;
		if (frameIndex >= numFrames) {
			frameIndex = 0;
		}
	}

	inline void Test1::Draw() {
		for (auto& sp : sps) {
			sp->Draw();
		}
		gLooper.GLBlendFunc(gLooper.blendDefault);

		for (size_t i = 0; i < 30000; i++) {
			auto v = gLooper.ShaderBegin(gLooper.shaderTexVert)
				//.Draw(gLooper.res_texFrenchFries, tex, 1);
				.Draw(gLooper.res_texSpineBoy, tex, 1);
			v->frameIndex = (int32_t)frameIndex;
			v->pos = 0;
			v->scale = 1.f;
		}

		gLooper.DrawNode(ui);
	}

	inline void Test1::OnWindowSizeChanged() {
		MakeUI();
	}
}
