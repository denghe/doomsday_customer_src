#pragma once

namespace Game {

	struct ChooseChar : Scene {
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
		// ...
		static constexpr XY charShowcaseAnchor{ 0, 1 };
		static constexpr XY charShowcasePos{ -1920 / 2 + margin + charIconSize + margin, charIcon1Pos.y };
		static constexpr float charShowcaseSize{ 768 };

		static constexpr XY charDetailAnchor{ 0, 1 };
		static constexpr XY charDetailPos{ charShowcasePos.x + charShowcaseSize + margin, charShowcasePos.y };
		static constexpr XY charDetailSize{ 1920 - margin * 4 - charIconSize - charShowcaseSize, charShowcaseSize };

		static constexpr XY selectBtnAnchor{ 0.5f, 0 };
		static constexpr XY selectBtnPos{ 0, -1080 / 2 + margin };


		xx::Shared<xx::Node> ui;
		xx::Node* charShowcaseContainer{};
		xx::Listi32<xx::FocusButton*> charIcons;
		int32_t charSelectedIndex{ -1 };
		void SelectChar(int32_t index);

		void MakeUI();

		void Init() override;
		void Update() override;
		void Draw() override;
		void OnWindowSizeChanged() override;
	};

}
