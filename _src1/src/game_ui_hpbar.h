#pragma once

namespace Game {

	struct UI_HPBar {
		// hp bar arguments
		static constexpr xx::UVRect hpbarCenter{ 2,2,4,4 };		// from res
		static constexpr float hpbarScale{ 1.f / 1920.f * Cfg::width };
		static constexpr float hpbarTexScale{ 4.f * hpbarScale };
		static constexpr XY hpbarSize{ 256.f * hpbarScale, 48 * hpbarScale };
		static constexpr XY hpbarPos{ Cfg::xy7.x + 10, Cfg::xy7.y - hpbarSize.y - 10 };	// left bottom
		static constexpr float hpbarContentBorderWidth{ 2 * hpbarTexScale };
		static constexpr XY hpbarContentPos{ hpbarPos.x + hpbarContentBorderWidth, hpbarPos.y + hpbarContentBorderWidth };	// left bottom
		static constexpr XY hpbarContentMaxSize{ hpbarSize - hpbarContentBorderWidth * 2 };
		static constexpr XY hpbarContentScale{ hpbarContentMaxSize / ResTpFrames::_size_ui_hpbar_content };

		xx::Scale9 s9;
		float hp{ 1 }, hpMax{ 2 };		// need change when update

		void Init();
		void Draw();
	};

}
