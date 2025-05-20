#pragma once

namespace Game {

	struct MainMenu : xx::SceneBase {
		xx::Camera camera;
		xx::Shared<xx::Node> ui;

		void Init() override;
		void Draw() override;
		void MakeUI();
		void OnWindowSizeChanged();
	};

}
