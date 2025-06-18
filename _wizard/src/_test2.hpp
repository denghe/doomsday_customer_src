#pragma once

namespace Game {

	inline void Test2::MakeUI() {
		ui.Emplace()->Init(0, {}, scale);
		ui->MakeChildren<xx::Button>()->Init(1, pos9 + XY{ -10, -10 }, anchor9
			, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});
		float x{ 10 };
		x += 10 + ui->MakeChildren<xx::Button>()->Init(1, pos7 + XY{ x, -10 }, anchor7
			, gLooper.btnCfg, U"M1", [&]() {
				SelectMonster<M1>();
			}).size.x;
		x += 10 + ui->MakeChildren<xx::Button>()->Init(1, pos7 + XY{ x, -10 }, anchor7
			, gLooper.btnCfg, U"M2", [&]() {
				SelectMonster<M2>();
			}).size.x;
		x += 10 + ui->MakeChildren<xx::Button>()->Init(1, pos7 + XY{ x, -10 }, anchor7
			, gLooper.btnCfg, U"M3", [&]() {
				SelectMonster<M3>();
			}).size.x;
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
		camera.SetOriginal({ 960, 540 });
		MakeUI();
		time = 1;
		SelectMonster<M1>();
	}

	inline void Test2::Update() {
		camera.Update(rnd, time);
		mousePos = camera.ToLogicPos(gLooper.mouse.pos);
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
		q->pos = scene->camera.ToGLPos(pos + f.spriteSize * 0.3f);
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

	template<typename T>
	inline void MBn<T>::Init(xx::Weak<T> shooter_, int32_t cannonIndex_, float scaleDurationSeconds_, float tarRadius_, float moveSpeed_) {
		scene = shooter_->scene;
		auto& ci = shooter_->cannonInfos[cannonIndex_];
		shooter = std::move(shooter_);
		cannonIndex = cannonIndex_;
		pos = ci.pos;
		radius = 0.f;
		radians = ci.radians;
		scaleDurationNumFrames = int32_t(scaleDurationSeconds_ * Cfg::fps);
		tarRadius = tarRadius_;
		radiusInc = (tarRadius_ - radius) / scaleDurationNumFrames;
		moveSpeedStep = moveSpeed_ * Cfg::frameDelay;
	}

	template<typename T>
	inline int32_t MBn<T>::Update() {
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
		posInc = { std::cosf(radians) * moveSpeedStep, std::sinf(radians) * moveSpeedStep };
		while (lifeEndTime >= scene->time) {
			pos += posInc;
			XX_YIELD_I(_n);
		};
		XX_END(_n);
		return 1;
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
			scene->mbs.Emplace().Emplace<MBn<M1>>()->Init(xx::WeakFromThis(this), i, 0.225f, 20.f, 111.f);
		}
	}

	inline void M1::Init(Test2* scene_) {
		scene = scene_;
		pos = { 960, 540 };
		radius = cRadius;
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
			for (_t = scene->time + int32_t(cShootBigDelaySeconds * Cfg::fps); _t >= scene->time;) {
				XX_YIELD_I(_n);
			}
		}
		XX_END(_n);
		return 0;
	}

	inline void M1::Draw() {
		// 4 cannon + 1 body
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

	XX_INLINE void M2::RotateTo(float radians_) {
		SetRadians(radians_);
		SyncCannonInfos();
	}

	XX_INLINE void M2::SyncCannonInfos() {
		// pos + XY{ cShootOffset.x * radiansCos - cShootOffset.y * radiansSin
		//	, cShootOffset.x * radiansSin + cShootOffset.y * radiansCos };
		auto c = radiansCos;
		auto s = radiansSin;
		auto ox = radius;
		auto oy = radius * -0.15f;
		cannonInfos[0].pos = pos + XY{ ox * c - oy * s, ox * s + oy * c };
		cannonInfos[0].radians = radians + 0.f;
		ox = radius;
		oy = radius * 0.15f;
		cannonInfos[1].pos = pos + XY{ ox * c - oy * s, ox * s + oy * c };
		cannonInfos[1].radians = radians + 0.f;
		ox = radius * 0.8f;
		oy = radius * 0.8f;
		cannonInfos[2].pos = pos + XY{ ox * c - oy * s, ox * s + oy * c };
		cannonInfos[2].radians = radians + float(M_PI * 2 * (15.f / 360));
		ox = radius * 0.8f;
		oy = radius * -0.8f;
		cannonInfos[3].pos = pos + XY{ ox * c - oy * s, ox * s + oy * c };
		cannonInfos[3].radians = radians + float(M_PI * 2 * (-15.f / 360));
	}

	XX_INLINE void M2::Shoot12() {
		scene->mbs.Emplace().Emplace<MBn<M2>>()->Init(xx::WeakFromThis(this), 0, 0.5f, 35.f, 400.f);
		scene->mbs.Emplace().Emplace<MBn<M2>>()->Init(xx::WeakFromThis(this), 1, 0.5f, 35.f, 400.f);
	}

	XX_INLINE void M2::Shoot34() {
		scene->mbs.Emplace().Emplace<MBn<M2>>()->Init(xx::WeakFromThis(this), 2, 0.225f, 20.f, 300.f);
		scene->mbs.Emplace().Emplace<MBn<M2>>()->Init(xx::WeakFromThis(this), 3, 0.225f, 20.f, 300.f);
	}

	inline void M2::Init(Test2* scene_) {
		scene = scene_;
		pos = { 960, 540 };
		radius = cRadius;
		SetRadians<true>(0.f);
		SyncCannonInfos();
	}

	inline int32_t M2::Update() {
		// aim mouse pos
		auto d = scene->mousePos - pos;
		RotateTo(std::atan2(d.y, d.x));

		XX_BEGIN(_n);
		while (true) {
			Shoot34();
			for (_t = scene->time + int32_t(cShootDelay34 * Cfg::fps); _t >= scene->time;) { XX_YIELD_I(_n); }
			Shoot34();
			for (_t = scene->time + int32_t(cShootDelay34 * Cfg::fps); _t >= scene->time;) { XX_YIELD_I(_n); }
			Shoot34();
			for (_t = scene->time + int32_t(cShootDelay34 * Cfg::fps); _t >= scene->time;) { XX_YIELD_I(_n); }
			Shoot12();
			for (_t = scene->time + int32_t(cShootDelay12 * Cfg::fps); _t >= scene->time;) { XX_YIELD_I(_n); }
			Shoot12();
			for (_t = scene->time + int32_t(cShootDelay12last * Cfg::fps); _t >= scene->time;) { XX_YIELD_I(_n); }
			for (_t = scene->time + int32_t(cShootBigDelay * Cfg::fps); _t >= scene->time;) { XX_YIELD_I(_n); }
		}
		XX_END(_n);
		return 0;
	}

	inline void M2::Draw() {
		// 4 cannon + 1 body
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

	XX_INLINE void M3::SyncCannonInfos() {
		auto c = radiansCos;
		auto s = radiansSin;
		auto ox = radius;
		auto oy = 0;
		cannonInfos[0].pos = pos + XY{ ox * c - oy * s, ox * s + oy * c };
		cannonInfos[0].radians = radians + 0.f;
	}

	XX_INLINE void M3::TryShoot() {
		if (nextShootTime >= scene->time) return;
		nextShootTime = scene->time + int32_t(cShootDelay * Cfg::fps);
		scene->mbs.Emplace().Emplace<MBn<M3>>()->Init(xx::WeakFromThis(this), 0, cBulletScaleDurations, cBulletRadius, cBulletMoveSpeed);
	}

	inline void M3::Init(Test2* scene_) {
		scene = scene_;
		pos = { 960, 540 };
		radius = cRadius;
		SetRadians<true>(0.f);
		SyncCannonInfos();
	}

	inline int32_t M3::Update() {
		XX_BEGIN(_n);
		while (true) {
			for (; pos.y > cMoveYRange.from; pos.y -= cMoveSpeed * Cfg::frameDelay) {
				SyncCannonInfos();
				TryShoot();
				XX_YIELD_I(_n);
			}
			for (; pos.y < cMoveYRange.to; pos.y += cMoveSpeed * Cfg::frameDelay) {
				SyncCannonInfos();
				TryShoot();
				XX_YIELD_I(_n);
			}
		}
		XX_END(_n);
		return 1;
	}

	inline void M3::Draw() {
		// 1 cannon + 1 body
		auto baseScale = radius * 2.f / gLooper.res._size_ui_gear.x * scene->camera.scale;
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res._texid_ui_gear, 2);
		// cannon
		{
			auto& f = *gLooper.res.ui_run3;
			q[0].pos = scene->camera.ToGLPos(cannonInfos[0].pos);
			q[0].anchor = { 0, 0.5f };
			q[0].scale = baseScale * 0.2f;
			q[0].radians = cannonInfos[0].radians;
			q[0].colorplus = stunEndTime >= scene->time ? 100000.f : 1.f;
			q[0].color = xx::RGBA8_White;
			q[0].texRect.data = f.textureRect.data;
		}
		// body
		auto& f = *gLooper.res.ui_gear;
		q[1].pos = scene->camera.ToGLPos(pos);
		q[1].anchor = 0.5f;
		q[1].scale = baseScale;
		q[1].radians = radians;
		q[1].colorplus = stunEndTime >= scene->time ? 100000.f : 1.f;
		q[1].color = xx::RGBA8_White;
		q[1].texRect.data = f.textureRect.data;
	}

}
