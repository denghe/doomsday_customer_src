#pragma once

// todo: hit show damage number  reduce hp

namespace Game {

	inline void Test3::MakeUI() {
		ui.Emplace()->Init(0, {}, scale);
		// todo
	}

	inline void Test3::OnWindowSizeChanged() {
		camera.SetBaseScale(scale);
		MakeUI();
	}

	inline void Test3::Init() {
		UpdateScale();
		MakeUI();

		grid.Init(32, 100, 100);
		effectTexts.Init(this, 100000);
		delta = Cfg::frameDelay;

		XY basePos{ grid.pixelSize * 0.5f };

		xx::CoutN(scale);
		camera.SetScale(scale, 0.6f);
		camera.SetOriginal(basePos);

		// init pathways
		/*
-300  0 300
	*1       -200
  /   \
*6     *2    -100
  \    /
	\/
    /\       0
  /    \
*3      *5   100
  \   /
	*4       200
		*/

		for (int32_t x = -1200; x < 1200; x += 50) {
			for (int32_t y = -600; y < 600; y += 50) {
				auto bp = basePos + XY{ x, y };
				std::vector<xx::CurvePoint> cps{ xx::CurvePoint
					{ bp + XY{ 0, -200} },
					{ bp + XY{ 300, -100} },
					{ bp + XY{ -300, 100} },
					{ bp + XY{ 0, 200} },
					{ bp + XY{ 300, 100} },
					{ bp + XY{ -300, -100} },
				};
				xx::MovePath mp;
				mp.FillCurve(true, cps);
				pathways.Emplace().Emplace()->Init(mp, 0.1f);
			}
		}

		// init snakes
		for (auto& pw : pathways) {
		snakes.Emplace().Emplace<Snake>()
			->Init<SnakeHead, SnakeBody, SnakeTail>(this, pw.pointer, 100);
		}

#if 0
		xx::CoutN(pathways.len);
#endif
	}

	XX_INLINE void Test3::Update_() {

		// hit check
		if (gLooper.mouse.PressedMBLeft()) {
			auto mp = camera.ToLogicPos(gLooper.mouse.pos);
#if 0
			for (auto e = snakes.len, i = 0; i < e; ++i) {
				auto& es = snakes[i]->elements;
				for (int32_t j = 0; j < es.len; ++j) {
					if (es[j]->HitCheck(mp)) {
						es[j]->Remove();	// unsafe
					}
				}
			}
#else
			auto cri = grid.PosToCRIndex(mp);
				grid.Foreach9All(cri.y, cri.x, [this, mp](decltype(grid)::Node& o) {
					if (o.value->HitCheck(mp, 16)) {
						//effectTexts.Add(o.cache.pos + XY{ 0, -o.cache.radius }, { 0, -1 }, xx::RGBA8_Red, 2, 123);
						effectTexts.Add(o.cache.pos, { 0, -1 }, xx::RGBA8_Red, 2.f, 123);
						o.value->Remove();	// unsafe
					}
				});
#endif
		}

		// range hit
		if (gLooper.KeyDownDelay(xx::KeyboardKeys::Space, 0.5f) || gLooper.mouse.PressedMBRight()) {
			static constexpr int32_t cHitRange{ 256 };
			auto cInnerRange = cHitRange - grid.cellSize * 0.5f * 1.4142f;
			auto mp = camera.ToLogicPos(gLooper.mouse.pos);
			auto cri = grid.PosToCRIndex(mp);
				grid.ForeachByRange(cri.y, cri.x, cHitRange, gLooper.rdd, [this, mp, cInnerRange](decltype(grid)::Node& o, float range) {
					if (o.cache.elementType == SnakeElementTypes::Head
						|| o.cache.elementType == SnakeElementTypes::Tail) return;
					if (range > cInnerRange) {
						auto d = mp - o.cache.pos;
						auto r = cHitRange + o.cache.radius;
						if (d.x * d.x + d.y * d.y > r * r) return;
					}
					//o.value->Remove();	// unsafe
					effectTexts.Add(o.cache.pos, { 0, -1 }, xx::RGBA8_Red, 2.f, 123);
			});
		}

		// update snakes
		for (auto i = snakes.len - 1; i >= 0; --i) {
			if (snakes[i]->Update()) {
				snakes.SwapRemoveAt(i);
			}
		}

		// update damage numbers
		effectTexts.Update();
	}

	XX_INLINE void Test3::Update() {
		if (gLooper.KeyDownDelay(xx::KeyboardKeys::Escape, 0.5f)
			|| gLooper.KeyDownDelay(xx::KeyboardKeys::Q, 0.5f)) {
			gLooper.DelaySwitchTo<MainMenu>();
		}

		assert(delta > 0);
		timePool += delta;
		while (timePool >= Cfg::frameDelay) {
			timePool -= Cfg::frameDelay;
			Update_();
			++time;
		}
	}


	inline void Test3::Draw() {
		// bg
		{
			auto& t = gLooper.res_bg_fight_1;
			auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(t, 1);
			q->pos = {};
			q->anchor = 0.5f;
			q->scale = lastWindowSize.y / t->Height() * camera.scale;
			q->radians = 0;
			q->colorplus = 0.5f;
			q->color = xx::RGBA8_White;
			q->texRect = { 0, 0, (uint16_t)t->Width(), (uint16_t)t->Height() };
		}

		// ui
		gLooper.DrawNode(ui);

#if 0
		// pathway
		xx::Quad q;
		q.SetFrame(gLooper.res.ui_button).SetColorplus(0.2f).SetScale(camera.scale);
		for (auto& pathway : pathways) {
			for (int32_t s = (int32_t)pathway->points.size(), i = 0; i < s; /*++i*/i += 100) {
				q.SetPosition(camera.ToGLPos(pathway->points[i].pos)).Draw();
			}
		}
#endif

		// snakes
		for (int32_t i = 0, e = snakes.len; i < e; ++i) {
			snakes[i]->Draw();
		}

		// todo: draw light?

		// todo: cut?

		// draw effect texts
		for (int32_t i = 0, e = effectTexts.ens.Count(); i < e; ++i) {
			auto& o = effectTexts.ens[i];
			//if (o.pos.x < areaMin.x || o.pos.x > areaMax.x || o.pos.y < areaMin.y || o.pos.y > areaMax.y) continue;
			o.Draw(this);
		}
	}

	/***********************************************************************************/

	inline void SnakeElement::PreInit(Snake* owner_, SnakeElementTypes elementType_) {
		owner = owner_;
		elementType = elementType_;
		index = owner_->elements.len - 1;
	}

	inline void SnakeElement::Init(float radius_) {
		radius = radius_;
		if (index > 0) {
			auto prev = GetPrev();
			assert(prev);
			auto cursorOffset = (int32_t)((prev->radius + radius) * cNodeDistanceRatio / owner->pathway->stepDistance);
			assert(cursorOffset > 0);
			pathwayCursor = prev->pathwayCursor + cursorOffset;
			auto ps = (int32_t)owner->pathway->points.size();
			while (pathwayCursor >= ps) {
				pathwayCursor -= ps;
			}
		}
		else {
			pathwayCursor = 0;
		}
		auto& p = owner->pathway->points[pathwayCursor];
		pos = p.pos;
		radians = -p.radians;
		owner->scene->grid.Add(gridIndex, this);	// grid sync
	}

	inline void SnakeElement::Remove() {
		assert(owner->elements[index].pointer == this);
		owner->scene->grid.Remove(gridIndex, this);	// grid sync
		auto& es = owner->elements;
		for (auto i = index + 1; i < es.len; ++i) {
			es[i]->index = i - 1;
		}
		auto idx = index;
		index = -1;
		es.RemoveAt(idx);	// unsafe
	}

	inline bool SnakeElement::HitCheck(XY p_, float hitRadius_) {
		if (elementType == SnakeElementTypes::Head || elementType == SnakeElementTypes::Tail)
			return false;
		auto d = p_ - pos;
		auto r = radius + hitRadius_;
		return (d.x * d.x + d.y * d.y < r * r);
	}

	inline SnakeElement* SnakeElement::GetPrev() {
		assert(owner->elements[index].pointer == this);
		if (index == 0) return {};
		return owner->elements[index - 1].pointer;
	}

	inline SnakeElement* SnakeElement::GetNext() {
		assert(owner->elements[index].pointer == this);
		if (index == owner->elements.len - 1) return {};
		return owner->elements[index + 1].pointer;
	}

	inline int32_t SnakeElement::BaseUpdate() {
		auto prev = GetPrev();
		assert(prev);
		auto cursorOffset = (int32_t)((prev->radius + radius) * cNodeDistanceRatio / owner->pathway->stepDistance);
		assert(cursorOffset > 0);
		auto cursor = pathwayCursor;
		if (prev->pathwayCursor > pathwayCursor) {
			cursor += (int32_t)owner->pathway->points.size();
		}
		if (std::abs(cursorOffset - (cursor - prev->pathwayCursor))
			< int32_t((cSpeed * cStickRatio) / owner->pathway->stepDistance)) {
			// follow
			pathwayCursor = prev->pathwayCursor + cursorOffset;
		}
		else {
			// backward
			pathwayCursor -= int32_t(cSpeed / owner->pathway->stepDistance);
			while (pathwayCursor < 0) {
				pathwayCursor += (int32_t)owner->pathway->points.size();
			}
		}
		auto ps = (int32_t)owner->pathway->points.size();
		while (pathwayCursor >= ps) {
			pathwayCursor -= ps;
		}
		auto& p = owner->pathway->points[pathwayCursor];
		pos = p.pos;
		radians = p.radians;

		owner->scene->grid.Update(gridIndex, this);	// grid sync
		return 0;
	}

	/***********************************************************************************/

	template<typename HeadType, typename BodyType, typename TailType>
	void Snake::Init(Test3* scene_, Pathway* pathway_, int32_t bodyLen_) {
		assert(elements.Empty());
		assert(bodyLen_ > 0);
		scene = scene_;
		pathway = pathway_;

		elements.Emplace().Emplace<TailType>()->PreInit(this, SnakeElementTypes::Tail);
		for (int32_t i = 0; i < bodyLen_; ++i) {
			elements.Emplace().Emplace<BodyType>()->PreInit(this, SnakeElementTypes::Body);
		}
		elements.Emplace().Emplace<HeadType>()->PreInit(this, SnakeElementTypes::Head);

		((TailType*)elements[0].pointer)->Init();
		for (int32_t i = 1; i <= bodyLen_; ++i) {
			((BodyType*)elements[i].pointer)->Init();
		}
		((HeadType*)elements[bodyLen_ + 1].pointer)->Init();
	}

	inline int32_t Snake::Update() {
		assert(elements.len);
		for (auto i = 0; i < elements.len; ++i) {
			if (elements[i]->Update()) {
				elements[i]->Remove();	// unsafe
			}
		}
		if (elements.Empty()) return -1;
		return 0;
	}

	inline void Snake::Draw() {
		for (int32_t i = 0, e = elements.len; i < e; ++i) {
			elements[i]->Draw();
		}
	}

	inline void Snake::DrawLight(float colorPlus_) {
		for (int32_t i = 0, e = elements.len; i < e; ++i) {
			elements[i]->DrawLight(colorPlus_);
		}
	}

	/***********************************************************************************/

	inline void SnakeHead::Init() {
		faceShaker.Shake(5, 300.f * Cfg::frameDelay, int32_t(0.5f * Cfg::fps), 99999999);
		SnakeElement::Init(16.f);
	}

	inline int32_t SnakeHead::Update() {
		faceShaker.Update(gLooper.rnd, gLooper.frameNumber);
		return BaseUpdate();
	}

	inline void SnakeHead::Draw() {
		auto& f1 = gLooper.res.snake_body;
		auto& f2 = gLooper.res.snake_face;
		assert(f1->tex == f2->tex);
		assert(f1->spriteSize.x == f2->spriteSize.x);

		auto& c = owner->scene->camera;
		auto p = c.ToGLPos(pos);
		auto s = radius * (2.f * cDrawScale) / f1->spriteSize.x * c.scale;
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(f1->tex, 2);

		q[0].pos = p;
		q[0].anchor = 0.5f;
		q[0].scale = s;
		q[0].radians = radians;
		q[0].colorplus = 1.f;
		q[0].color = xx::RGBA8_White;
		q[0].texRect.data = f1->textureRect.data;

		q[1].pos = p + faceShaker.offset;
		q[1].anchor = 0.5f;
		q[1].scale = s;
		q[1].radians = radians;
		q[1].colorplus = 1.f;
		q[1].color = xx::RGBA8_White;
		q[1].texRect.data = f2->textureRect.data;
	}

	inline void SnakeHead::DrawLight(float colorPlus_) {
		// todo
	}

	/***********************************************************************************/

	inline void SnakeBody::Init() {
		auto d = cRadiusRange.to - cRadiusRange.from;
		auto len = owner->elements.len - 2;
		auto step = d / len;
		auto r = cRadiusRange.from + step * (index - 1);
		// auto r = gLooper.rnd.Next<float>(cRadius.from, cRadius.to);
		SnakeElement::Init(r);
	}

	inline void SnakeBody::U1_RadiusAnim() {
		XX_BEGIN(U1_n);
	LabBig:
		for (; radius < cRadiusRange.to; radius += cRadiusAnimStep) {
			XX_YIELD(U1_n);
		}
		for (; radius > cRadiusRange.from; radius -= cRadiusAnimStep) {
			XX_YIELD(U1_n);
		}
		goto LabBig;
		XX_END(U1_n);
	}

	inline int32_t SnakeBody::Update() {
		U1_RadiusAnim();
		return BaseUpdate();
	}

	inline void SnakeBody::Draw() {
		auto& f = gLooper.res.snake_body;
		auto& c = owner->scene->camera;
		xx::Quad{}.SetFrame(f)
			.SetPosition(c.ToGLPos(pos))
			.SetRotate(radians)
			.SetScale(radius * (2.f * cDrawScale) / f->spriteSize.x * c.scale)
			.Draw();
	}

	inline void SnakeBody::DrawLight(float colorPlus_) {
		// todo
	}

	/***********************************************************************************/

	inline void SnakeTail::Init() {
		SnakeElement::Init(16.f);
	}

	inline int32_t SnakeTail::Update() {
		// todo: do not move?
		pathwayCursor += int32_t(cSpeed / owner->pathway->stepDistance);
		auto ps = (int32_t)owner->pathway->points.size();
		while (pathwayCursor >= ps) {
			pathwayCursor -= ps;
		}
		auto& p = owner->pathway->points[pathwayCursor];
		pos = p.pos;
		radians = p.radians;
		owner->scene->grid.Update(gridIndex, this);
		return 0;
	}

	inline void SnakeTail::Draw() {
		auto& f = gLooper.res.snake_tail;
		auto& c = owner->scene->camera;
		xx::Quad{}.SetFrame(f)
			.SetPosition(c.ToGLPos(pos))
			.SetRotate(radians)
			.SetScale(radius * (2.f * cDrawScale) / f->spriteSize.x * c.scale)
			.Draw();
	}

	inline void SnakeTail::DrawLight(float colorPlus_) {
		// todo
	}

}
