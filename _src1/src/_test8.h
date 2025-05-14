#pragma once

namespace Game {

	struct Test8 : Stage {
		int32_t numNumbers{};
		void MakeUI();
		void Init() override;
		void OnWindowSizeChanged() override;
		void Update() override;
		void DrawCustomUI() override;
	};

}
