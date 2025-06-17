#pragma once

namespace Game {

	inline void Test2::MakeUI() {
		ui.Emplace()->Init(0, {}, scale);
		ui->MakeChildren<xx::Button>()->Init(1, pos9 + XY{ -10, -10 }, anchor9
			, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});
		ui->MakeChildren<xx::Button>()->Init(1, pos7 + XY{ 10, -10 }, anchor7
			, gLooper.btnCfg, U"A", [&]() {
				SelectMonster<M1>();
			});
	}

	template<typename T>
	void Test2::SelectMonster() {
		mbs.Clear();
		ms.Clear();
		auto o = xx::MakeShared<T>();
		o->Init(this);
		ms.Emplace(std::move(o));
	}

	inline void Test2::Init() {
		UpdateScale();
		camera.SetBaseScale(scale);
		camera.SetOriginal({960, 540});
		MakeUI();
		time = 1;
		SelectMonster<M1>();
	}

	inline void Test2::Update() {
		camera.Update(rnd, time);
		for (auto i = mbs.len - 1; i >= 0; --i) {
			auto& o = mbs[i];
			if (o->Update()) {
				mbs.SwapRemoveAt(i);
			}
		}
		for (auto i = ms.len - 1; i >= 0; --i) {
			auto& o = ms[i];
			if (o->Update()) {
				ms.SwapRemoveAt(i);
			}
		}
		++time;
	}

	inline void Test2::Draw() {
		// draw bg
		xx::Quad().SetFrame(gLooper.res.ui_dot).SetScale(lastWindowSize).SetColor({ 249, 181, 81, 255 }).Draw();

		for (auto& o : mbs) o->Draw();
		for (auto& o : mbs) o->Draw1();
		for (auto& o : mbs) o->Draw2();
		for (auto& o : mbs) o->Draw3();
		for (auto& o : ms) o->Draw();

		gLooper.DrawNode(ui);
	}

	inline void Test2::OnWindowSizeChanged() {
		camera.SetBaseScale(scale);
		MakeUI();
	}

	/********************************************************************************/

	template<bool force>
	XX_INLINE void M::SetRadians(float radians_) {
		if constexpr (!force) {
			if (radians == radians_) return;
		}
		radians = radians_;
		radiansCos = std::cosf(radians_);
		radiansSin = std::sinf(radians_);
	}

	/********************************************************************************/

	inline void MB::Draw() {
		auto& f = *gLooper.res.ui_circle;
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(f.tex, 1);
		q->pos = scene->camera.ToGLPos(pos + XY{ 12, 12 });
		q->anchor = *f.anchor;
		q->scale = radius * 2.f / f.spriteSize.x * scene->camera.scale;
		q->radians = radians;
		q->colorplus = 1.f;
		q->color = { 127, 127, 127, 127 };
		q->texRect.data = f.textureRect.data;
	}

	inline void MB::Draw1() {
		auto& f = *gLooper.res.ui_circle;
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(f.tex, 1);
		q->pos = scene->camera.ToGLPos(pos);
		q->anchor = *f.anchor;
		q->scale = radius * 2.f / f.spriteSize.x * scene->camera.scale;
		q->radians = radians;
		q->colorplus = 1.f;
		q->color = { 47, 6, 1, 255 };
		q->texRect.data = f.textureRect.data;
	}

	inline void MB::Draw2() {
		auto& f = *gLooper.res.ui_circle;
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(f.tex, 1);
		q->pos = scene->camera.ToGLPos(pos);
		q->anchor = *f.anchor;
		q->scale = radius * 2.f / f.spriteSize.x * scene->camera.scale * 0.8f;
		q->radians = radians;
		q->colorplus = 1.f;
		q->color = { 248, 125, 22, 255 };
		q->texRect.data = f.textureRect.data;
	}

	inline void MB::Draw3() {
		auto& f = *gLooper.res.ui_circle;
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(f.tex, 1);
		q->pos = scene->camera.ToGLPos(pos);
		q->anchor = *f.anchor;
		q->scale = radius * 2.f / f.spriteSize.x * scene->camera.scale * 0.4f;
		q->radians = radians;
		q->colorplus = 1.f;
		q->color = { 252, 250, 241, 255 };
		q->texRect.data = f.textureRect.data;
	}


	/********************************************************************************/

	XX_INLINE void M1::RotateTo(float radians_) {
		SetRadians(radians_);
		SyncCannonInfos();
	}

	XX_INLINE void M1::SyncCannonInfos() {
		// pos + XY{ cShootOffset.x * radiansCos - cShootOffset.y * radiansSin
		//	, cShootOffset.x * radiansSin + cShootOffset.y * radiansCos };
		// 1, 0
		cannonInfos[0].pos = pos + XY{ radius * radiansCos, radius * radiansSin };
		cannonInfos[0].radians = radians + 0.f;
		// 0, 1
		cannonInfos[1].pos = pos + XY{ -radius * radiansSin, radius * radiansCos };
		cannonInfos[1].radians = radians + float(M_PI) * 0.5f;
		// -1, 0
		cannonInfos[2].pos = pos + XY{ -radius * radiansCos, -radius * radiansSin };
		cannonInfos[2].radians = radians + float(M_PI);
		// 0, -1
		cannonInfos[3].pos = pos + XY{ radius * radiansSin, -radius * radiansCos };
		cannonInfos[3].radians = radians + float(M_PI) * 1.5f;
	}

	XX_INLINE void M1::Shoot() {
		for (int32_t i = 0; i < 4; ++i) {
			scene->mbs.Emplace().Emplace<MB1>()->Init(xx::WeakFromThis(this), i);
		}
	}

	inline void M1::Init(Test2* scene_) {
		scene = scene_;
		pos = {960, 540};
		radius = 100.f;
		SetRadians<true>(0.f);
		SyncCannonInfos();
	}

	inline int32_t M1::Update() {
		XX_BEGIN(_n);
		while (true) {
			for (_i = 0; _i < cShootNums; ++_i) {
				Shoot();
				for (_t = scene->time + cShootDelayNumFrames; _t >= scene->time;) {
					RotateTo(radians + cRadiansStep);
					XX_YIELD_I(_n);
				}
			}
			RotateTo(0);
			for (_t = scene->time + cShootBigDelayNumFrames; _t >= scene->time;) {
				XX_YIELD_I(_n);
			}
		}
		XX_END(_n);
		return 0;
	}

	inline void M1::Draw() {
		// 4 cannon + 1 body
		// todo
		auto baseScale = radius * 2.f / gLooper.res._size_ui_gear.x * scene->camera.scale;
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res._texid_ui_gear, 5);
		// cannons
		for (int32_t i = 0; i < 4; ++i) {
			auto& f = *gLooper.res.ui_run3;
			q[i].pos = scene->camera.ToGLPos(cannonInfos[i].pos);
			q[i].anchor = { 0, 0.5f };
			q[i].scale = baseScale * 0.1f;
			q[i].radians = cannonInfos[i].radians;
			q[i].colorplus = stunEndTime >= scene->time ? 100000.f : 1.f;
			q[i].color = xx::RGBA8_White;
			q[i].texRect.data = f.textureRect.data;
		}
		// body
		auto& f = *gLooper.res.ui_gear;
		q[4].pos = scene->camera.ToGLPos(pos);
		q[4].anchor = 0.5f;
		q[4].scale = baseScale;
		q[4].radians = radians;
		q[4].colorplus = stunEndTime >= scene->time ? 100000.f : 1.f;
		q[4].color = xx::RGBA8_White;
		q[4].texRect.data = f.textureRect.data;
	}

	/********************************************************************************/

	inline void MB1::Init(xx::Weak<M1> shooter_, int32_t cannonIndex_) {
		scene = shooter_->scene;
		auto& ci = shooter_->cannonInfos[cannonIndex_];
		shooter = std::move(shooter_);
		cannonIndex = cannonIndex_;
		pos = ci.pos;
		radius = 0.f;
		tarRadius = cRadius;
		radiusInc = (tarRadius - radius) * c1_ScaleDurationNumFrames;
		radians = ci.radians;
	}

	inline int32_t MB1::Update() {
		// todo: hit player check
		XX_BEGIN(_n);
		while (radius < tarRadius) {
			if (!shooter) {
				// todo: death effect
				return 1;
			}
			radius += radiusInc;
			pos = shooter->cannonInfos[cannonIndex].pos;
			radians = shooter->cannonInfos[cannonIndex].radians;
			XX_YIELD_I(_n);
		}
		radius = tarRadius;
		lifeEndTime = scene->time + int32_t(Cfg::fps * 10.f);
		posInc = { std::cosf(radians) * cMoveSpeedStep, std::sinf(radians) * cMoveSpeedStep };
		while (lifeEndTime >= scene->time) {
			pos += posInc;
			XX_YIELD_I(_n);
		};
		XX_END(_n);
		return 1;
	}

}
