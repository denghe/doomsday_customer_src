#pragma once

namespace Game {
	
	struct VertAnim {
		xx::Ref<xx::GLTexture> tex;
		xx::Ref<xx::GLVertTexture> vertTex;

		XY pos{}, scale{ 1 };
		int32_t numFrames{};
		float frameIndex{};

		void Update();
		void Draw();
	};

	struct Test1 : Scene {
		xx::Shared<xx::Node> ui;
		void MakeUI();

		xx::Listi32<xx::Shared<xx::SpinePlayer>> sps;
		xx::Listi32<xx::Shared<VertAnim>> vas;

		void Init() override;
		void Update() override;
		void Draw() override;
		void OnWindowSizeChanged() override;
	};

}
