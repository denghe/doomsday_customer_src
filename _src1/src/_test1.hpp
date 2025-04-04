#pragma once

namespace Game {

	struct DeathEffectTest : DrawableEx {
		float totalScale{1}, scaleX{1};
		int32_t n1{}, n2{};

		void Init(Stage* stage_, XY pos_ = {}) {
			stage = stage_;
			pos = pos_;
		}

		int32_t Update1() {
			XX_BEGIN(n1);
			for (totalScale = 1.f; totalScale >= 0.f; totalScale -= 1.f / (Cfg::fps * 0.5)) {
				XX_YIELD_I(n1);
			}
			XX_END(n1);
			return 1;
		}

		void Update2() {
			XX_BEGIN(n2);
			for (scaleX = 1.f; scaleX > -1.f; scaleX -= 1.f / (Cfg::fps * 0.1)) {
				XX_YIELD(n2);
			}
			for (scaleX = -1.f; scaleX < 1.f; scaleX += 1.f / (Cfg::fps * 0.1)) {
				XX_YIELD(n2);
			}
			XX_END(n2);
		}

		int32_t Update() override {
			if (Update1()) return 1;
			Update2();
			return 0;
		}

		void Draw() override {
			auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
				.Draw(gLooper.res._tex_monster_chips->GetValue(), 1);
			q->pos = pos;
			q->anchor = ResTpFrames::_anchor_monster_chips;
			q->scale = { totalScale * scaleX, totalScale };
			q->radians = 0;
			q->colorplus = 1.f;
			q->color = xx::RGBA8_White;
			q->texRect.data = gLooper.res._uvrect_monster_chips.data;
		}
	};

	/***********************************************************************************************/
	/***********************************************************************************************/


	inline void Test1::Init() {
		ui.Emplace()->Init();

		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy7m + XY{ 10, -10 }
			, Cfg::xy7a, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
		});

		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy8m + XY{ 0, -10 }
			, Cfg::xy8a, gLooper.btnCfg, U"test death effect", [&]() {
				auto x = gLooper.rnd.Next<float>(-Cfg::width_2 + 100, Cfg::width_2 - 100);
				auto y = gLooper.rnd.Next<float>(-Cfg::height_2 + 100, Cfg::height_2 - 100);
				items.Emplace().Emplace<DeathEffectTest>()->Init(this, {x, y});
		});

		camera.SetMaxFrameSize(Cfg::unitSize);
		camera.SetScale(Cfg::defaultScale);
	}

	inline void Test1::Update() {
		for(auto i = items.len - 1; i >=0; --i) {
			if (items[i]->Update()) {
				items.SwapRemoveAt(i);
			}
		}
	}

	inline void Test1::Draw() {
		xx::Quad().SetFrame(gLooper.res.ui_menu).SetRotate(M_PI_2).SetScale(Cfg::width / ResTpFrames::_size_ui_menu.x).Draw();
		gLooper.DrawNode(ui);

		for (auto e = items.len, i = 0; i < e; ++i) {
			items[i]->Draw();
		}
	}
}
