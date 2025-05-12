#pragma once

namespace Game {

	inline void FlashPoint::Init(Stage* stage_, XY const& pos_) {
		stage = stage_;
		pos = pos_;
		alpha = {};
		radians = stage_->rnd.Next<float>(-M_PI*0.05, M_PI*0.05);
	}

	inline int32_t FlashPoint::Update() {
		XX_BEGIN(n);
		// fade in
		for (alpha = 0.f; alpha < 1.f; alpha += 1.f / (Cfg::fps * 0.5f)) {
			XX_YIELD_I(n);
		}
		alpha = 1.f;
		// sleep
		for (i = stage->time + int32_t(Cfg::fps * 0.2f); stage->time < i;) {
			XX_YIELD_I(n);
		}
		// fade out
		for (alpha = 1.f; alpha > 0.f; alpha -= 1.f / (Cfg::fps * 0.5f)) {
			XX_YIELD_I(n);
		}
		alpha = 0.f;
		// quit
		return 1;
		XX_END(n);
		return 0;
	}

	inline void FlashPoint::Draw() {
		auto& q = *gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(gLooper.res._texid_ef_starlight, 1);
		q.pos = stage->camera.ToGLPos(pos);
		q.anchor = 0.5f;
		q.scale = 0.25f * stage->camera.scale;
		q.radians = radians;
		q.colorplus = 1.f;
		q.color = { 255,255,255, (uint8_t)(255*alpha) };
		q.texRect.data = ResTpFrames::_uvrect_ef_starlight.data;
	}




	inline void Test4::Init() {
		StageInit({60, 60});
		player.Emplace<Player_1>()->Init(this);

		ui.Emplace()->Init();

		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy7m + XY{ 10, -10 }
			, Cfg::xy7a, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});


		// fill flash area data
		xx::Data flashAreaData;
		xx::FrameBuffer fb(true);
		constexpr XYi wh{ ResTpFrames::_size_bullet_coin5 };
		fb.Draw(wh, true, xx::RGBA8_White, [this] {
			xx::Quad().SetFrame(gLooper.res.bullet_coin5).Draw();
		}, &flashAreaData);
		//xx::CoutN(flashAreaData);

		auto colors = (uint32_t*)flashAreaData.buf;
		for (int y = 0; y < wh.y; ++y) {
			for (int x = 0; x < wh.x; ++x) {
				auto i = y * wh.x + x;
				auto c = colors[i];
				// ff 0c 0f 11 == ABGR( little endian? )
				if (c == 0xff0c0f11) {
					flashPosIndexs.Emplace(xx::UV{(uint16_t)x, (uint16_t)y});
				}
			}
		}

		camera.scale = 3;
	}

	inline void Test4::Update() {
		StageUpdate();

		// update flash points
		for (auto i = flashPoints.len - 1; i >= 0; --i) {
			if (flashPoints[i].Update()) {
				flashPoints.SwapRemoveAt(i);
			}
		}

		// make some flash point
		for (auto i = 0; i < 10; ++i) {
			auto uv = flashPosIndexs[rnd.Next<int32_t>(flashPosIndexs.len)];
			flashPoints.Emplace().Init(this, player->pos + XY{ uv.u, uv.v });
		}
	}

	inline void Test4::DrawCustomUI() {

		for (auto e = flashPoints.len, i = 0; i < e; ++i) {
			flashPoints[i].Draw();
		}

	}
}
