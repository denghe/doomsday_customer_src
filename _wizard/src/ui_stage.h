#pragma once

namespace Game {
	struct Stage;
}

namespace UI {

	struct HPBar : xx::Node {
		uint32_t v1{}, v2{};
		xx::RGBA8 color{};
		void Draw() override;
	};


	// stage's base
	struct Stage {

		void Init(xx::Weak<Game::Stage> stage_);
		void OnWindowSizeChanged();
		void Draw();

		void SetRoundInfo(std::u32string_view const& txt_);
		void SetRoundInfo(std::string_view const& txt_);
		void SetRoundInfo();

	protected:
		xx::Weak<Game::Stage> stage;
		xx::Shared<xx::Node> root;
		xx::Shared<xx::Label> gameTips;
		xx::Shared<xx::Label> roundInfo;
		xx::Shared<HPBar> hpBar, mpBar;
		void MakeDevelopUI();
		void MakeUI();
	};

}
