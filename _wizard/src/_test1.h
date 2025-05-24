#pragma once

namespace Game {
	
	struct Test1 : Scene {
		xx::Shared<xx::Node> ui;
		void MakeUI();

		xx::Shared<xx::SpinePlayer> sp;

		void Init() override;
		void Update() override;
		void Draw() override;
		void OnWindowSizeChanged() override;
	};

}
