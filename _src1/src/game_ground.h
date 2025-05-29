#pragma once

namespace Game {

	struct Ground1 : Ground {
		xx::Ref<xx::Frame> frame;
		XY scale{};
		xx::RGB8 color{};
		XY size{};
		XYi gridSize{};
		std::unique_ptr<uint8_t[]> colors;

		void Init(Stage* stage_, xx::Ref<xx::Frame> const& frame_, XY scale_ = 1.f, xx::RGB8 color_ = xx::RGB8_White);
		void AddSomeGrass(float ratio, int32_t typeId) override;
		void Update() override;
		void Clear() override;
		void Draw() override;
	};

	struct Ground2 : Ground {
		xx::Ref<xx::GLTiledTexture> tex;

		void Init(Stage* stage_);
		void AddSomeGrass(float ratio, int32_t typeId) override;
		void Update() override;
		void Clear() override;
		void Draw() override;
	};

}
