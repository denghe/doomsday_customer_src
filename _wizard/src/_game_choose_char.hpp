#pragma once

namespace Game {

	struct CharNode : xx::Node {
		xx::Ref<xx::Frame> frame;

		void Init(int z_, XY position_, XY anchor_, XY size_, xx::Ref<xx::Frame> frame_) {
			Node::Init(z_, position_, 1, anchor_, size_);
			frame = std::move(frame_);
		}

		void Draw() override {
			assert(frame->spriteSize.y == 512);
			XY pos{ worldMinXY.x + size.x * 0.5f * worldScale.x, worldMinXY.y };
			auto scale = worldScale * (size.y / 512.f);
			xx::Quad().SetFrame(frame).SetScale(scale).SetPosition(pos).SetAnchor({0.5f, 0.f}).Draw();
			// todo: draw weapon
		}
	};

	inline void ChooseChar::MakeUI() {
		/*
		      [title]
----------------------------------
[] [      ] [ desc ...			 ]
[] [      ] [					 ]
[] [ char ] [					 ]
[] [      ] [					 ]
[] [      ] [					 ]
version     [choose]     esc/space

width detail:
margin spacing [char icon] spacing margin [ char anim ] margin [ desc ] margin
		*/

		static constexpr float margin{ 30 };
		static constexpr float spacing{ 5 };
		static constexpr float charIconCoreSize{ 128 };
		static constexpr float charIconSize{ charIconCoreSize + spacing * 2 };
		static constexpr float charImageWidth{ (1920 - margin * 4 - charIconSize) / 2 };
		static constexpr float charDescWidth{ charImageWidth };

		static constexpr XY titlePos{ 0, 1080 / 2 - margin };
		static constexpr XY titleAnchor{ 0.5f, 1.f };

		static constexpr XY linePos{ 0, titlePos.y - 90 };
		static constexpr XY lineAnchor{ 0.5f, 0.f };
		static constexpr XY lineSize{ 1920 - margin * 2, 5 };

		static constexpr XY charIconAnchor{ 0, 1 };
		static constexpr XY charIcon1Pos{ -1920 / 2 + margin, linePos.y - margin };
		//static constexpr XY charIcon2Pos{ charIcon1Pos.x, charIcon1Pos.y - charIconSize - margin };
		//static constexpr XY charIcon3Pos{ charIcon2Pos.x, charIcon2Pos.y - charIconSize - margin };
		// ...

		ui.Emplace()->Init(0, {}, scale);

		// title
		ui->MakeChildren<xx::Label>()->Init(1, titlePos, 2, titleAnchor, xx::RGBA8_White, UI::TextOf_title_ChooseHero());

		// line
		ui->MakeChildren<xx::Image2>()->Init(1, linePos, lineAnchor, lineSize, false, gLooper.res.ui_circle);

		// char icons
		for(int i = 0; i < 3; ++i) {
			XY p{ charIcon1Pos.x, charIcon1Pos.y - (charIconSize + margin) * i };
			auto& b = ui->MakeChildren<xx::FocusButton>()->Init(1, p, charIconAnchor, charIconSize
				, gLooper.cfg_imgBtnNormal, gLooper.cfg_imgBtnHighlight);
			b.MakeChildren<CharNode>()->Init(2, spacing, 0, charIconCoreSize, gLooper.res.char_[i]);
			b.onFocus = [] { gLooper.sound.Play(gLooper.res_sound_button_1); };
			b.onClicked = [] {  };	// todo
		}

		// version
		ui->MakeChildren<xx::Label>()->Init(1, pos1 + XY{ 30, 30 }, 1, anchor1, xx::RGBA8_Gray, U"ver 0.1 beta");

		// todo
	}

	inline void ChooseChar::OnWindowSizeChanged() {
		MakeUI();
	}

	inline void ChooseChar::Init() {
		UpdateScale();
		MakeUI();
	}

	void ChooseChar::Update() {
	}

	inline void ChooseChar::Draw() {
		gLooper.DrawNode(ui);
	}

}
