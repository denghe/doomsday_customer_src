#pragma once

namespace Game {

	// main menu, stage's base
	struct Scene : xx::SceneBase {
		Camera camera;
		int32_t time{};
		float timePool{}, delta{};

		XY lastWindowSize{};					// physics size
		XY size{};								// design size
		XY size_2{};							// design size / 2
		float scale{};							// base scale. design size * scale = physics size
		
		/*
			screen design anchor point
		   ┌───────────────────────────────┐
		   │ 7             8             9 │
		   │                               │
		   │                               │
		   │ 4             5             6 │
		   │                               │
		   │                               │
		   │ 1             2             3 │
		   └───────────────────────────────┘
		*/
		XY pos7{}, pos8{}, pos9{};
		XY pos4{}, pos5{}, pos6{};
		XY pos1{}, pos2{}, pos3{};

		static constexpr XY anchor7{ 0, 1 }, anchor8{ 0.5, 1 }, anchor9{ 1, 1 };
		static constexpr XY anchor4{ 0, 0.5 }, anchor5{ 0.5, 0.5 }, anchor6{ 1, 0.5 };
		static constexpr XY anchor1{ 0, 0 }, anchor2{ 0.5, 0 }, anchor3{ 1, 0 };

		virtual void OnWindowSizeChanged() {};	// override for UI update layout
		void UpdateScale();						// need call when Init
		void BeforeUpdate() override;			// call UpdateSize + OnWindowSizeChanged
	};

}
