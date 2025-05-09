#pragma once

namespace Game {

	inline void CreatureGroup::Init(Stage* stage_, XY const& pos_, float radians_, float speed_, int32_t len) {
		stage = stage_;
		frame = gLooper.res.monster_hamburger;
		pos = pos_;
		radians = radians_;
		scale = 1;
		distance = cMaxDistance;
		distanceStep = speed_ * Cfg::frameDelay;
		vec = { std::cosf(radians), std::sinf(radians) };
		endLifeTime = stage->time + (int32_t)(Cfg::fps * 60);
		for (int32_t i = 0; i < len; ++i) {
			memberPoss.Emplace(pos + vec * i * distance);
		}
	}

	inline int32_t CreatureGroup::Update() {
		auto len = memberPoss.len;
		XX_BEGIN(n);

		// pull tail
		do {
			XX_YIELD_I(n);
			distance -= distanceStep;
			for (int32_t i = 0; i < len; ++i) {
				memberPoss[i] = pos + vec * i * distance;
			}
		} while (distance > cMinDistance);

		// move head
		do {
			XX_YIELD_I(n);
			distance += distanceStep;
			pos -= vec * distanceStep * len;
			for (int32_t i = 0; i < len; ++i) {
				memberPoss[i] = pos + vec * i * distance;
			}
		} while (distance < cMaxDistance);

		if (stage->time > endLifeTime) 
			return 1;
		XX_YIELD_I_TO_BEGIN(n);
		XX_END(n);
		return 0;
	}

	inline void CreatureGroup::Draw() {
		auto len = memberPoss.len;
		auto midIdx = len / 2;

		auto a = frame->anchor.has_value() ? *frame->anchor : XY{ 0.5f, 0.5f };
		XY s{ scale * stage->camera.scale };
		XY sa{ a.x, 0 };								// for shadow 
		XY ss{ s.x, -s.y * 0.3f };						//
		// todo: radians
		auto cp = whiteColorEndTime >= stage->time ? 10000.f : 1.f;
		auto fuv = frame->textureRect.data;
		auto qs = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(frame->tex, len * 2);
		int32_t qi{};
		for (int32_t i = 0; i < len; ++i) {				// shadow
			auto& q = qs[qi++];
			q.pos = stage->camera.ToGLPos(memberPoss[i]);
			q.anchor = sa;
			q.scale = ss;
			q.radians = 0;
			q.colorplus = cp;
			q.color = { 0,0,0,127 };
			q.texRect.data = fuv;
		}
		for (int32_t i = len - 1; i > midIdx; --i) {	// tail
			auto& q = qs[qi++];
			q.pos = stage->camera.ToGLPos(memberPoss[i]);
			q.anchor = a;
			q.scale = s;
			q.radians = 0;
			q.colorplus = cp;
			q.color = color;
			q.texRect.data = fuv;
		}
		for (int32_t i = 0; i <= midIdx; ++i) {			// head to body
			auto& q = qs[qi++];
			q.pos = stage->camera.ToGLPos(memberPoss[i]);
			q.anchor = a;
			q.scale = s;
			q.radians = 0;
			q.colorplus = cp;
			q.color = color;
			q.texRect.data = fuv;
		}
	}

	inline void Test3::Init() {
		fb.Init();
		ui.Emplace()->Init();

		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy7m + XY{ 10, -10 }
			, Cfg::xy7a, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});

		gridSize = { 60, 60 };
		mapSize = 128 * gridSize;

		ground.Emplace()->Init(this, mapSize, gLooper.res.ground_cell2);
		player.Emplace<Player_1>()->Init(this);

		camera.scale = Cfg::defaultScale;
		camera.mapSize = mapSize;
		camera.newOriginal = camera.original = mapSize * 0.5f;
	}

	inline void Test3::Update() {
		// scale control
		if (gLooper.KeyDownDelay(xx::KeyboardKeys::Z, 0.02f)) {
			camera.IncreaseScale(0.01f, 5);
		}
		else if (gLooper.KeyDownDelay(xx::KeyboardKeys::X, 0.02f)) {
			camera.DecreaseScale(0.01f, 0.1f);
		}

		player->Update();
		camera.SetOriginal<true>(player->pos, camera.ToLogicPos(gLooper.mouse.pos));
		camera.Update();

		for (auto i = cgs.len - 1; i >= 0; --i) {
			if (cgs[i]->Update()) {
				cgs.SwapRemoveAt(i);
			}
		}
		// ...

		auto speed = rnd.Next<float>(100, 300);
		auto radians = rnd.Next<float>(-M_PI, M_PI);
		auto len = rnd.Next<int32_t>(5, 8);
		cgs.Emplace().Emplace()->Init(this, player->pos, radians, speed, len);


		++time;
	}

	inline void Test3::Draw() {
		// draw floor
		ground->Draw();

		// calculate display cut area
		auto areaMin = camera.ToLogicPos({ -gLooper.width_2 - Cfg::unitSize * 2, gLooper.height_2 + Cfg::unitSize * 2 });
		auto areaMax = camera.ToLogicPos({ gLooper.width_2 + Cfg::unitSize * 2, -gLooper.height_2 - Cfg::unitSize * 2 });

		// draw game items ( order by y )
		// 
		// prepare
		auto& yd = gLooper.yDraws;

		//yd.Emplace(player->pos.y, player.pointer);

		for (auto e = cgs.len, i = 0; i < e; ++i) {
			auto& o = cgs[i];
			if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
			yd.Emplace(o->pos.y, o.pointer);
		}

		// sort
		std::sort(yd.buf, yd.buf + yd.len, [](auto& a, auto& b) { return a.first < b.first; });

		// draw
		for (auto e = yd.len, i = 0; i < e; ++i) {
			yd[i].second->Draw();
		}

		// clean up
		yd.Clear();


		gLooper.DrawNode(ui);
	}
}
