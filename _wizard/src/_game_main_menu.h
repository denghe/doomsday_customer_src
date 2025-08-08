#pragma once

namespace Game {

	struct MainMenu : Scene {
		xx::Shared<xx::Node> ui;
		xx::Listi32<xx::FocusButton*> buttons;
		int32_t selectedButtonIndex{};
		void SelectMenu(int32_t idx);

		void MakeUI();

		void Init() override;
		void Update() override;
		void Draw() override;
		void OnWindowSizeChanged() override;
	};

}
