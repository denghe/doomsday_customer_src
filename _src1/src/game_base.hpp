#pragma once

namespace Game {

	inline void Drawable::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(frame->tex, 2);
		XY s{ scale * stage->camera.scale };
		if (needFlipX) s.x = -s.x;
		// body
		q[1].pos = stage->camera.ToGLPos(pos);
		q[1].anchor = frame->anchor.has_value() ? *frame->anchor : XY{ 0.5f, 0.5f };
		q[1].scale = s;
		q[1].radians = 0;
		q[1].colorplus = whiteColorEndTime >= stage->time ? 10000.f : 1.f;
		q[1].color = xx::RGBA8_White;
		q[1].texRect.data = frame->textureRect.data;
		// shadow
		q[0].scale = { q[1].scale.x, q[1].scale.y * 0.2f };
		q[0].pos = { q[1].pos.x, q[1].pos.y - q[0].scale.y * frame->spriteSize.y * 0.5f };
		q[0].anchor = { q[1].anchor.x, 0.3 };
		q[0].radians = q[1].radians;
		q[0].colorplus = q[1].colorplus;
		q[0].color = { 0,0,0,127 };
		q[0].texRect.data = q[1].texRect.data;
	}

	inline XY Stage::GetPlayerBornPos() {
		return mapSize * 0.5f;
	}

	inline void Stage::ForceLimit(XY& pos) {
		if (pos.x < 0.f) pos.x = 0.f;
		else if (pos.x >= mapSize.x)
			pos.x = mapSize.x - std::numeric_limits<float>::epsilon();
		if (pos.y < 0.f) pos.y = 0.f;
		else if (pos.y >= mapSize.y)
			pos.y = mapSize.y - std::numeric_limits<float>::epsilon();
	}

	inline bool Stage::IsOutOfMap(XY const& pos) {
		return pos.x < 0 || pos.y < 0 || pos.x >= mapSize.x || pos.y >= mapSize.y;
	}

	inline XY Stage::GetRndPosDoughnut(float maxRadius, float safeRadius) {
		auto len = maxRadius - safeRadius;
		auto len_radius = len / maxRadius;
		auto safeRadius_radius = safeRadius / maxRadius;
		auto radius = std::sqrtf(rnd.Next<float>(0, len_radius) + safeRadius_radius) * maxRadius;
		auto radians = rnd.Next<float>(-M_PI, M_PI);
		return { std::cosf(radians) * radius, std::sinf(radians) * radius };
	}


	inline void Stage::Update() {
		// scale control
		if (gLooper.KeyDownDelay(xx::KeyboardKeys::Z, 0.02f)) {
			camera.IncreaseScale(0.01f, 5);
		}
		else if (gLooper.KeyDownDelay(xx::KeyboardKeys::X, 0.02f)) {
			camera.DecreaseScale(0.01f, 0.1f);
		}

		// update player bullets
		for (auto i = playerBullets.len - 1; i >= 0; --i) {
			auto& o = playerBullets[i];
			if (o->Update()) {
				playerBullets.SwapRemoveAt(i);
			}
		}

		// update player
		player->Update();

		// update monsters
		for (auto i = monsters.items.len - 1; i >= 0; --i) {
			auto& o = monsters.items[i];
			if (o->Update()) {
				monsters.Remove(o);
			}
		}

		// update monster generators
		for (auto i = monsterGenerators.len - 1; i >= 0; i--) {
			auto& mg = monsterGenerators[i];
			if (mg->activeTime <= time) {
				if (mg->destroyTime >= time) {
					mg->Update();
				}
				else {
					monsterGenerators.SwapRemoveAt(i);
				}
			}
		}

		// update effects
		etm.Update();
		for (auto i = effects.len - 1; i >= 0; --i) {
			auto& o = effects[i];
			if (o->Update()) {
				effects.SwapRemoveAt(i);
			}
		}

		// ... more updates

		// sync cam
		camera.SetOriginal(player->pos);

		// update time
		++time;

		// finish check
		if (monsterGenerators.Empty() && monsters.items.Empty()) {
			onCleanup();
		}
	}

	inline void Stage::Draw() {
		// draw floor
		ground->Draw();

		// draw game items ( order by y )
		{
			// prepare
			auto& yd = gLooper.yDraws;
			yd.Emplace(player->pos.y, player.pointer);
			for (auto e = playerBullets.len, i = 0; i < e; ++i) {
				auto& o = playerBullets[i];
				yd.Emplace(o->pos.y, o.pointer);
			}
			for (auto e = monsters.items.len, i = 0; i < e; ++i) {
				auto& o = monsters.items[i];
				yd.Emplace(o->pos.y, o.pointer);
			}
			for (auto e = effects.len, i = 0; i < e; ++i) {
				auto& o = effects[i];
				yd.Emplace(o->pos.y, o.pointer);
			}
			// sort
			std::sort(yd.buf, yd.buf + yd.len, [](auto& a, auto& b) {
				return a.first < b.first;
				});
			// draw
			for (auto e = yd.len, i = 0; i < e; ++i) {
				yd[i].second->Draw();
			}
			// clean up
			yd.Clear();
		}

		etm.Draw();

		// draw ui
		gLooper.DrawNode(ui);

		// draw tips
		gLooper.ctcDefault.Draw({ 0, gLooper.windowSize_2.y - 5 }
			, xx::ToString("zoom: ZX  move: ASDW  m = ", monsters.items.len
				, " b = ", playerBullets.len
				, " e = ", etm.ens.Count()), xx::RGBA8_Green, { 0.5f, 1 });
	}


	inline void Creature::Idle() {
		XX_BEGIN(idle_lineNumber);
		for (scale.y = 1.f; scale.y >= IdleCfg::cEnd; scale.y -= IdleCfg::cInc) {
			scale.x = IdleCfg::cTotal - scale.y;
			XX_YIELD(idle_lineNumber);
		}
		for (; scale.y <= 1.f; scale.y += IdleCfg::cInc) {
			scale.x = IdleCfg::cTotal - scale.y;
			XX_YIELD(idle_lineNumber);
		}
		XX_YIELD_TO_BEGIN(idle_lineNumber);
		XX_END(idle_lineNumber);
	}

	void Monster::Knowckback(float speed, XY const& d) {
		knockback = true;
		knockbackSpeed = speed * (1.f / Cfg::fps);
		auto dd = d.x * d.x + d.y * d.y;
		auto mag = std::sqrt(dd);
		knockbackDist = d / mag;
		knockbackReduceValuePerFrame = knockbackSpeed * (1.f / (Cfg::fps * 0.5f));
	}

	int32_t Monster::Update() {
		if (knockback) {
			knockbackSpeed -= knockbackReduceValuePerFrame;
			if (knockbackSpeed <= 0) {
				knockback = false;
			}
			else {
				pos = pos + knockbackDist * knockbackSpeed;
				stage->ForceLimit(pos);
				stage->monsters.Update(this);	// sync space index
			}
		}
		else {
			// move to player
			auto p = stage->player.pointer;
			auto pp = p->pos;
			auto d = pp - pos;
			auto dd = d.x * d.x + d.y * d.y;
			auto r2 = p->radius + radius;
			if (dd < r2 * r2) {
				// cross with player?
				//p->Hurt(damage, d);   // todo
			}
			else {
				d = pp - pos + tarOffset;
				auto dd = d.x * d.x + d.y * d.y;
				if (dd < radius * radius) {
					// reached offset point? reset offset point
					tarOffset = stage->GetRndPosDoughnut(tarOffsetRadius, 0.1f);
				}
				// calc & move
#if 0
				// slowly than mag normalize
				auto r = std::atan2f(d.y, d.x);
				pos += XY{ std::cosf(r) * moveSpeed, std::sinf(r) * moveSpeed };
#else
				// faster than atan2 + sin cos  1/4
				auto mag = std::sqrt(dd);
				auto norm = d / mag;
				pos += norm * moveSpeed;
#endif
				stage->ForceLimit(pos);
				stage->monsters.Update(this);	// sync space index
			}

			Idle();	// always play this  anim
		}

		// todo
		return destroyTime <= stage->time;
	}

	int32_t Monster::Hurt(float dmg, XY const& txtD, XY const& knockbackD) {
		dmg = std::ceilf(dmg);
		if (hp <= dmg) {
			// dead
			stage->etm.Add(pos + frame->spriteSize * XY{ 0, -0.5f }, txtD, xx::RGBA8_Red, 6, hp);
			stage->effects.Emplace().Emplace<EffectDeath>()->Init(stage, frame, pos);
			stage->monsters.Remove(this);
			return 1;
		}
		else {
			// hurt
			hp -= dmg;
			stage->etm.Add(pos + frame->spriteSize * XY{ 0, -0.5f }, txtD, xx::RGBA8_Yellow, 5, dmg);
			whiteColorEndTime = stage->time + int32_t(0.1f * Cfg::fps);
			Knowckback(500.f, knockbackD);
			return 0;
		}
	}

	inline void MonsterGen::Init(Stage* stage_, int activeTime_, int destroyTime_, float generateNumsPerSeconds_) {
		stage = stage_;
		activeTime = activeTime_;
		destroyTime = destroyTime_;
		countIncPerFrame = generateNumsPerSeconds_ / Cfg::fps;
	}

	template<typename T>
	inline void MonsterGen::CreateFromRect() {
		auto& p = stage->player->pos;
		auto& rnd = stage->rnd;
		static constexpr auto ds_2 = Cfg::designSize / 2;
		for (countPool += countIncPerFrame; countPool >= 1; --countPool) {
			if (stage->monsters.items.len < Cfg::numMaxMonsters) {
				XY pos{};
				switch (stage->rnd.Next<int32_t>(0, 4)) {
				case 0:
					pos = { p.x - ds_2.x - Cfg::unitSize
						, p.y + rnd.Next<float>(-ds_2.y - Cfg::unitSize, ds_2.y + Cfg::unitSize) };
					break;
				case 1:
					pos = { p.x + ds_2.x + Cfg::unitSize
						, p.y + rnd.Next<float>(-ds_2.y - Cfg::unitSize, ds_2.y + Cfg::unitSize) };
					break;
				case 2:
					pos = { p.x + rnd.Next<float>(-ds_2.x - Cfg::unitSize, ds_2.x + Cfg::unitSize)
						, p.y - ds_2.y - Cfg::unitSize };
					break;
				case 3:
					pos = { p.x + rnd.Next<float>(-ds_2.x - Cfg::unitSize, ds_2.x + Cfg::unitSize)
						, p.y + ds_2.y + Cfg::unitSize };
					break;
				}

				auto m = xx::MakeShared<T>();
				m->Init(stage, pos);
				stage->monsters.Add(std::move(m));
			}
		}
	}

	template<typename T>
	inline void MonsterGen::CreateFromDoughnut() {
		auto& p = stage->player->pos;
		auto& rnd = stage->rnd;
		for (countPool += countIncPerFrame; countPool >= 1; --countPool) {
			if (stage->monsters.items.len < Cfg::numMaxMonsters) {
				auto pos = stage->player->pos + stage->GetRndPosDoughnut(Cfg::designSize.x, 300.f);
				auto m = xx::MakeShared<T>();
				m->Init(stage, pos);
				stage->monsters.Add(std::move(m));
			}
		}
	}

}
