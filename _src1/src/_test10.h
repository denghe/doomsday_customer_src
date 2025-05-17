#pragma once

namespace Game {

	struct Test10 : Stage {
		xx::Ref<xx::GLTexture> tex;
		XY offset{};

		void MakeUI();
		void Init() override;
		void OnWindowSizeChanged() override;
		void Update() override;
		void DrawCustomUI() override;
	};

}
