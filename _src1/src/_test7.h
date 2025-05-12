#pragma once

namespace Game {

	struct Test7 : Stage {
		xx::Listi32<std::pair<xx::Ref<xx::Frame>, XY>> outlines;

		XY uvOffset{};
		void MakeUI();
		void Init() override;
		void OnWindowSizeChanged() override;
		void Update() override;
		void DrawCustomUI() override;

		void AddMoreOutline();
	};

}
