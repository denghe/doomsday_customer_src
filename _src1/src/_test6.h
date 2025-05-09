#pragma once

namespace Game {

	template<int32_t len>
	struct alignas(8) Trail {
		Stage* stage{};
		using D = std::pair<XY, XY>;
		std::array<D, len> data;
		int32_t cursor{};	// data[cursor] == first node
		float radius{};
		D& At(int32_t idx);
		void StepCursor();
		void Init(Stage* stage_, XY headPos_, float radius_);
		void Update(XY headPos_);
		void Draw();
	};

	struct Test6 : Stage {
		xx::Listi32<Trail<(int32_t)Cfg::fps>> trails;

		void MakeUI();
		void Init() override;
		void OnWindowSizeChanged() override;
		void Update() override;
		void DrawCustomUI() override;
	};

}
