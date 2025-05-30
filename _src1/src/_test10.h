#pragma once

namespace Game {

	struct Test10 : Stage {
		void MakeUI();
		void Init() override;
		void OnWindowSizeChanged() override;
		void Update() override;
		void DrawCustomUI() override;
	};

}
