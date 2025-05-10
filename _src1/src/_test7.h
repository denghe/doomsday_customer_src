#pragma once

namespace Game {

	struct Test7 : Stage {
		float offsetX{};
		void MakeUI();
		void Init() override;
		void OnWindowSizeChanged() override;
		void Update() override;
		void DrawCustomUI() override;
	};

}
