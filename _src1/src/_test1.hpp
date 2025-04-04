#pragma once

namespace Game {

	struct DeathEffectTest : DrawableEx {
		xx::Ref<xx::Frame> frame;
		float totalScale{1}, scaleX{1};
		int32_t animBeginTime{};
		int32_t n0{}, n1{}, n2{};

		void Init(Stage* stage_, xx::Ref<xx::Frame> frame_, XY pos_ = {}) {
			frame = std::move(frame_);
			stage = stage_;
			pos = pos_;
			animBeginTime = stage_->time + int32_t(Cfg::fps * 0.5f);
		}

		int32_t Update1() {
			XX_BEGIN(n1);
			for (totalScale = 1.f; totalScale >= 0.f; totalScale -= 1.f / (Cfg::fps * 0.7)) {
				XX_YIELD_I(n1);
			}
			XX_END(n1);
			return 1;
		}

		void Update2() {
			XX_BEGIN(n2);
			for (scaleX = 1.f; scaleX > -1.f; scaleX -= 1.f / (Cfg::fps * 0.06)) {
				XX_YIELD(n2);
			}
			for (scaleX = -1.f; scaleX < 1.f; scaleX += 1.f / (Cfg::fps * 0.06)) {
				XX_YIELD(n2);
			}
			XX_YIELD_TO_BEGIN(n2);
			XX_END(n2);
		}

		int32_t Update() override {
			if (stage->time < animBeginTime) return 0;
			if (Update1()) return 1;
			Update2();
			return 0;
		}

		void Draw() override {
			auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
				.Draw(frame->tex, 1);
			q->pos = pos;
			q->anchor = frame->anchor.has_value() ? frame->anchor.value() : XY{0.5f, 0.5f};
			q->scale = { totalScale * scaleX, totalScale };
			q->radians = 0;
			q->colorplus = 1.f;
			q->color = xx::RGBA8_White;
			q->texRect.data = frame->textureRect.data;
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
				tasks.Add([](Test1* self)->xx::Task<>{
					for (auto i = 0; i < 100; ++i) {
						auto x = gLooper.rnd.Next<float>(-Cfg::width_2 + 100, Cfg::width_2 - 100);
						auto y = gLooper.rnd.Next<float>(-Cfg::height_2 + 100, Cfg::height_2 - 100);
						auto idx = gLooper.rnd.Next<int32_t>(0, self->frames.len);
						self->items.Emplace().Emplace<DeathEffectTest>()->Init(self, self->frames[idx], { x, y });
						co_await gLooper.AsyncSleep(0.1f);
					}
				}(this));
		});

		camera.SetMaxFrameSize(Cfg::unitSize);
		camera.SetScale(Cfg::defaultScale);

		frames.Emplace(gLooper.res.monster_chips);
		frames.Emplace(gLooper.res.monster_cola);
		frames.Emplace(gLooper.res.monster_hamburger);
		frames.Emplace(gLooper.res.monster_instantnoodles);
		frames.Emplace(gLooper.res.monster_laptop);
		frames.Emplace(gLooper.res.monster_roastduck);
		frames.Emplace(gLooper.res.monster_sofa);

		frames.Emplace(gLooper.res.bullet_cash);
		frames.Emplace(gLooper.res.bullet_coin10);
		frames.Emplace(gLooper.res.bullet_coin5);

		frames.Emplace(gLooper.res.player1);
		frames.Emplace(gLooper.res.player2);
		// ...
	}

	inline void Test1::Update() {
		for(auto i = items.len - 1; i >=0; --i) {
			if (items[i]->Update()) {
				items.SwapRemoveAt(i);
			}
		}
		++time;
	}

	inline void Test1::Draw() {
		xx::Quad().SetFrame(gLooper.res.ui_menu).SetRotate(M_PI_2).SetScale(Cfg::width / ResTpFrames::_size_ui_menu.x).Draw();
		gLooper.DrawNode(ui);

		for (auto e = items.len, i = 0; i < e; ++i) {
			items[i]->Draw();
		}
	}
}
