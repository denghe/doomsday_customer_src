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
		StageInit({ 60, 60 });
		player.Emplace<Player_1>()->Init(this);

		ui.Emplace()->Init();

		ui->MakeChildren<xx::Button>()->Init(1, Cfg::xy7m + XY{ 10, -10 }
			, Cfg::xy7a, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});
	}

	inline void Test3::Update() {
		StageUpdate();

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
	}


	inline void Test3::DrawCustomOrderYItem(xx::Listi32<std::pair<float, Game::Drawable*>>& yd, XY areaMin, XY areaMax) {

		for (auto e = cgs.len, i = 0; i < e; ++i) {
			auto& o = cgs[i];
			if (o->pos.x < areaMin.x || o->pos.x > areaMax.x || o->pos.y < areaMin.y || o->pos.y > areaMax.y) continue;
			yd.Emplace(o->pos.y, o.pointer);
		}

	}
}
