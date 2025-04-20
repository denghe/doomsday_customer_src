#pragma once

namespace Game {
	
	struct FlashPoint {
		Stage* stage{};
		XY pos{};
		float alpha{};
		float radians{};
		int32_t i{};
		int32_t n{};
		void Init(Stage* stage_, XY const& pos_);
		int32_t Update();
		void Draw();
	};

	struct Test4 : Stage {
		xx::Shared<xx::Node> ui;
		xx::Listi32<FlashPoint> flashPoints;
		xx::Listi32<int32_t> flashPosIndexs;

		void Init() override;
		void Update() override;
		void Draw() override;
	};

}
