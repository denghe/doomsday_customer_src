#pragma once

namespace Game {

	struct Test9 : Stage {
		int32_t numHPBars{};
		void MakeUI();
		void Init() override;
		void OnWindowSizeChanged() override;
		void Update() override;
		void DrawCustomUI() override;
	};

}
