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
		float radians{};
		xx::Listi32<xx::Shared<xx::SpinePlayer>> sps;
		xx::Listi32<xx::Shared<VertAnim>> vas;

		xx::Ref<xx::GLVertTexture> vtSpineBoy;
		xx::Ref<xx::GLVertTexture> vtFrenchFries;
		xx::Ref<xx::GLVertTexture> vtSlime;

		void AddVertAnim(xx::Ref<xx::GLTexture> tex, xx::Ref<xx::GLVertTexture> vtex, float scale = 1.f);
		void AddSomeSpineBoy(int32_t n);
		void AddSomeFrenchFries(int32_t n);
		void AddSomeSlime(int32_t n);

		void Init() override;
		void Update() override;
		void Draw() override;
		void OnWindowSizeChanged() override;
	};

}
