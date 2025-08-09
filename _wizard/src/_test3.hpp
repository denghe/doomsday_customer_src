#pragma once

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

		XY basePos{400, 300};

		camera.SetBaseScale(scale);
		camera.SetScale(2.f);
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

		std::vector<xx::CurvePoint> cps{ xx::CurvePoint
			{ basePos + XY{ 0, -200} },
			{ basePos + XY{ 300, -100} },
			{ basePos + XY{ -300, 100} },
			{ basePos + XY{ 0, 200} },
			{ basePos + XY{ 300, 100} },
			{ basePos + XY{ -300, -100} },
		};
		xx::MovePath mp;
		mp.FillCurve(true, cps);
		pathway.Init(mp, 0.1f);

		// init snakes
		snakes.Emplace().Emplace<Snake>()
			->Init<SnakeHead, SnakeBody, SnakeTail>(this, &pathway, 10);
	}

	inline void Test3::Update() {
		if (gLooper.KeyDownDelay(xx::KeyboardKeys::Escape, 0.5f)) {
			gLooper.DelaySwitchTo<MainMenu>();
		}

		// hit check	// todo: use space idnex
		if (gLooper.mouse.PressedMBLeft()) {
			auto mp = camera.ToLogicPos(gLooper.mouse.pos);
			for (auto e = snakes.len, i = 0; i < e; ++i) {
				auto& es = snakes[i]->elements;
				for (int32_t j = 0; j < es.len; ++j) {
					if (es[j]->HitCheck(mp)) {
						es[j]->Remove();
						goto LabEnd;
					}
				}
			}
		}
		LabEnd:

		for (auto i = snakes.len - 1; i >= 0; --i) {
			if (snakes[i]->Update()) {
				snakes.SwapRemoveAt(i);
			}
		}

	}

	inline void Test3::Draw() {
		gLooper.DrawNode(ui);

		// draw pathway
		xx::Quad q;
		q.SetFrame(gLooper.res.ui_button).SetColorplus(0.2f).SetScale(camera.scale);
		for (int32_t s = (int32_t)pathway.points.size(), i = 0; i < s; /*++i*/i += 100) {
			q.SetPosition(camera.ToGLPos(pathway.points[i].pos)).Draw();
		}

		for (int32_t i = 0, e = snakes.len; i < e; ++i) {
			snakes[i]->Draw();
		}

		// todo: draw light?
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
			auto cursorOffset = (int32_t)((prev->radius + radius) / owner->pathway->stepDistance);
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
	}

	inline void SnakeElement::Remove() {
		assert(owner->elements[index].pointer == this);
		auto& es = owner->elements;
		for (auto i = index + 1; i < es.len; ++i) {
			es[i]->index = i - 1;
		}
		auto idx = index;
		index = -1;
		es.RemoveAt(idx);	// unsafe
	}

	inline bool SnakeElement::HitCheck(XY p) {
		if (elementType == SnakeElementTypes::Head || elementType == SnakeElementTypes::Tail)
			return false;
		auto d = p - pos;
		return (d.x * d.x + d.y * d.y < radius * radius);
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
		auto cursorOffset = (int32_t)((prev->radius + radius) / owner->pathway->stepDistance);
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
		radians = -p.radians;
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
		SnakeElement::Init(16.f);
	}

	inline int32_t SnakeHead::Update() {
		return BaseUpdate();
	}

	inline void SnakeHead::Draw() {
		auto& f = gLooper.res.ui_circle;
		auto& c = owner->scene->camera;
		xx::Quad{}.SetFrame(f)
			.SetColorplus(1.f)
			.SetPosition(c.ToGLPos(pos))
			.SetRotate(radians)
			.SetScale(radius * 2.f / f->spriteSize.x * c.scale)
			.Draw();
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
		auto& f = gLooper.res.ui_circle;
		auto& c = owner->scene->camera;
		xx::Quad{}.SetFrame(f)
			.SetColorplus(0.5f)
			.SetPosition(c.ToGLPos(pos))
			.SetRotate(radians)
			.SetScale(radius * 2.f / f->spriteSize.x * c.scale)
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
		radians = -p.radians;
		return 0;
	}

	inline void SnakeTail::Draw() {
		auto& f = gLooper.res.ui_circle;
		auto& c = owner->scene->camera;
		xx::Quad{}.SetFrame(f)
			.SetColorplus(0.2f)
			.SetPosition(c.ToGLPos(pos))
			.SetRotate(radians)
			.SetScale(radius * 2.f / f->spriteSize.x * c.scale)
			.Draw();
	}

	inline void SnakeTail::DrawLight(float colorPlus_) {
		// todo
	}

}
