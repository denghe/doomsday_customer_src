#pragma once

namespace Game {
	
	struct Test1 : Stage {
		void MakeUI();
		void Init() override;
		void OnWindowSizeChanged() override;
	};

}
