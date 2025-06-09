#pragma once

namespace Game {

	struct MainMenu : Scene {
		xx::Shared<xx::Node> ui;
		void MakeUI();

		float offset{};

		void Init() override;
		void Update() override;
		void Draw() override;
		void OnWindowSizeChanged() override;
	};

}
