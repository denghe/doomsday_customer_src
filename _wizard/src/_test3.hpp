#pragma once

namespace Game {

	inline void Test3::MakeUI() {
		ui.Emplace()->Init(0, {}, scale);
		// todo
	}

	inline void Test3::OnWindowSizeChanged() {
		MakeUI();
	}

	inline void Test3::Init() {
		UpdateScale();
		MakeUI();

		// init pathways
		auto basePos = pos5;
		std::vector<xx::CurvePoint> cps{ xx::CurvePoint
			{ basePos + XY{ -300, -180} },
			{ basePos + XY{ 300, -180} },
			{ basePos + XY{ 300, 180} },
			{ basePos + XY{ -300, 180} }
		};
		xx::MovePath mp;
		mp.FillCurve(true, cps);
		pathway.Init(mp, 0.1f);

		// init snakes
		snakes.Emplace().Emplace<Snake>()->Init<SnakeHead, SnakeBody, SnakeTail>(this, &pathway, 3);
	}

	inline void Test3::Update() {
		if (gLooper.KeyDownDelay(xx::KeyboardKeys::Escape, 0.5f)) {
			gLooper.DelaySwitchTo<MainMenu>();
		}

		for (auto i = snakes.len - 1; i >= 0; --i) {
			if (snakes[i]->Update()) {
				snakes.SwapRemoveAt(i);
			}
		}
	}

	inline void Test3::Draw() {
		gLooper.DrawNode(ui);

		xx::Quad q;
		q.SetFrame(gLooper.res.ui_button);
		for (auto& p : pathway.points) {
			q.SetPosition(p.pos).SetColorplus(0.2f).Draw();
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
			es[i]->index = i + 1;
		}
		auto idx = index;
		index = -1;
		es.RemoveAt(idx);	// unsafe
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
		// todo: do not follow? 
		SnakeElement::Init(radius);
		return 0;
	}

	inline void SnakeHead::Draw() {
		xx::Quad{}.SetFrame(gLooper.res.ui_run3).SetPosition(pos).SetRotate(radians)
			.SetScale(radius * (1.f / 16.f)).Draw();
	}

	inline void SnakeHead::DrawLight(float colorPlus_) {
		// todo
	}

	/***********************************************************************************/

	inline void SnakeBody::Init() {
		SnakeElement::Init(16.f);
	}

	inline int32_t SnakeBody::Update() {
		// todo: do not follow?
		SnakeElement::Init(radius);
		return 0;
	}

	inline void SnakeBody::Draw() {
		xx::Quad{}.SetFrame(gLooper.res.ui_run2).SetPosition(pos).SetRotate(radians)
			.SetScale(radius * (1.f / 16.f)).Draw();
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
		pathwayCursor += 100;
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
		xx::Quad{}.SetFrame(gLooper.res.ui_run1).SetPosition(pos).SetRotate(radians)
			.SetScale(radius * (1.f / 16.f)).Draw();
	}

	inline void SnakeTail::DrawLight(float colorPlus_) {
		// todo
	}

}
