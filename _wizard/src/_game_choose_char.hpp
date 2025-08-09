#pragma once

namespace Game {

	struct CharNode : xx::Node {
		xx::Ref<xx::Frame> fChar, fWeapon;

		void Init(int z_, XY position_, XY anchor_, XY size_, xx::Ref<xx::Frame> fChar_, xx::Ref<xx::Frame> fWeapon_) {
			Node::Init(z_, position_, 1, anchor_, size_);
			fChar = std::move(fChar_);
			fWeapon = std::move(fWeapon_);
		}

		void Draw() override {
			XY cPos{ worldMinXY.x + size.x * 0.35f * worldScale.x, worldMinXY.y };
			XY wPos{ cPos.x, cPos.y + worldSize.y * 0.3f };
			xx::Quad().SetFrame(fChar).SetScale(worldSize.y / fChar->spriteSize.y).SetPosition(cPos).SetAnchor({ 0.5f, 0.f }).SetRotate(0).Draw()
				.SetFrame(fWeapon).SetScale(worldSize.y / fWeapon->spriteSize.x * 1.333f).SetPosition(wPos).SetAnchor({ 0.333f, 0.5f }).SetRotate(-M_PI_4).Draw();
		}
	};

	inline void ChooseChar::SelectChar(int32_t idx) {
		{
			auto& o = charIcons[gLooper.charSelectedIndex];
			o->alwaysHighlight = false;
			o->ApplyCfg();
		}
		auto& o = charIcons[idx];
		o->alwaysHighlight = true;
		o->ApplyCfg();
		gLooper.charSelectedIndex = idx;
		auto cn = (CharNode*)o->children[1].pointer;
		charShowcaseContainer->Clear();
		charShowcaseContainer->MakeChildren<CharNode>()->Init(1, {}, charShowcaseAnchor, charShowcaseSize, cn->fChar, cn->fWeapon);
	}

	inline void ChooseChar::Choose() {
		// todo
		gLooper.DelaySwitchTo<Stage>();
	}

	inline void ChooseChar::MakeUI() {
		// cleanup
		charIcons.Clear();

		// make || remake
		ui.Emplace()->Init(0, {}, scale);

		// title
		ui->MakeChildren<xx::Label>()->Init(1, titlePos, 2, titleAnchor, xx::RGBA8_White, UI::TextOf_title_ChooseHero());

		// line
		ui->MakeChildren<xx::Image2>()->Init(1, linePos, lineAnchor, lineSize, false, gLooper.res.ui_circle);

		// char icons
		for(int i = 0; i < charCount; ++i) {
			XY p{ charIcon1Pos.x, charIcon1Pos.y - (charIconSize + margin) * i };
			auto& b = ui->MakeChildren<xx::FocusButton>()->Init(1, p, charIconAnchor, charIconSize
				, gLooper.cfg_imgBtnNormal, gLooper.cfg_imgBtnHighlight);
			b.MakeChildren<CharNode>()->Init(2, spacing, 0, charIconCoreSize, gLooper.res.char_[i], gLooper.res.weapon_[i]);
			b.onFocus = [i = i, this] { gLooper.sound.Play(gLooper.res_sound_button_1); SelectChar(i); };
			b.onClicked = [] {  };	// todo
			charIcons.Emplace(&b);
		}

		// char showcase
		charShowcaseContainer = &ui->MakeChildren<xx::Node>()->Init(1, charShowcasePos);

		// select default or last
		SelectChar(gLooper.charSelectedIndex);

		// char detail	// todo: rich text
		ui->MakeChildren<xx::Scale9Sprite>()->Init(1, charDetailPos, charDetailAnchor, charDetailSize, *gLooper.cfg_imgBtnNormal);

		// select button
		{
			auto& b = ui->MakeChildren<xx::FocusLabelButton>()->Init(1, selectBtnPos, selectBtnAnchor
				, gLooper.cfg_btnNormal, gLooper.cfg_btnHighlight, UI::TextOf_generic_Choose());
			b.onFocus = [] { gLooper.sound.Play(gLooper.res_sound_button_1); };
			b.onClicked = [this] { Choose(); };
		}

		// version
		ui->MakeChildren<xx::Label>()->Init(1, pos1 + XY{ 30, 30 }, 1, anchor1, xx::RGBA8_Gray, U"ver 0.1 beta");

		// esc back
		{
			XY p{ pos3 + XY{ -30, 30 } };
			auto& L1 = ui->MakeChildren<xx::Label>()->Init(1, p, 1, anchor3, xx::RGBA8_White, UI::TextOf_generic_SpaceChoose());
			p.x -= L1.GetScaledSize().x;
			auto& L2 = ui->MakeChildren<xx::Label>()->Init(1, p, 1, anchor3, xx::RGBA8_White, " | ");
			p.x -= L2.GetScaledSize().x;
			auto& L3 = ui->MakeChildren<xx::Label>()->Init(1, p, 1, anchor3, xx::RGBA8_White, UI::TextOf_generic_EscBack());
		}
	}

	inline void ChooseChar::OnWindowSizeChanged() {
		MakeUI();
	}

	inline void ChooseChar::Init() {
		UpdateScale();
		MakeUI();
	}

	void ChooseChar::Update() {
		if (gLooper.KeyDownDelay(xx::KeyboardKeys::Space, 0.15)) {
			Choose();
		}
		else
		if (gLooper.KeyDownDelay(xx::KeyboardKeys::S, 0.15)) {
			auto idx = gLooper.charSelectedIndex + 1;
			if (idx >= charCount) idx = 0;
			SelectChar(idx);
		}
		else
		if (gLooper.KeyDownDelay(xx::KeyboardKeys::W, 0.15)) {
			auto idx = gLooper.charSelectedIndex - 1;
			if (idx < 0) idx = charCount - 1;
			SelectChar(idx);
		}
		if (gLooper.KeyDownDelay(xx::KeyboardKeys::Escape, 0.5f)) {
			gLooper.DelaySwitchTo<MainMenu>();
		}
	}

	inline void ChooseChar::Draw() {
		gLooper.DrawNode(ui);
	}

}
