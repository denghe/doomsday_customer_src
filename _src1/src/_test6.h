#pragma once

namespace Game {

	struct Test6 : Stage {
		void MakeUI();
		void Init() override;
		void OnWindowSizeChanged() override;
		void Draw() override;
	};

}
