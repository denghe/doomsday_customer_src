#pragma once

namespace Game {
	
	struct Test1 : Scene {
		xx::Shared<xx::Node> ui;
		void MakeUI();

		xx::Listi32<xx::Shared<xx::SpinePlayer>> sps;
		xx::Ref<xx::GLTexture> tex;
		int32_t numFrames{};
		int32_t frameIndex{};

		void Init() override;
		void Update() override;
		void Draw() override;
		void OnWindowSizeChanged() override;
	};

}
