#pragma once

namespace Game {

	struct Stage;
	struct UI_HPBar {
		// hp bar arguments
		static constexpr xx::UVRect hpbarCenter{ 2,2,4,4 };		// from res
		static constexpr float hpbarScale{ 1.f / 1080.f * Cfg::height };
		static constexpr float hpbarTexScale{ 4.f * hpbarScale };
		static constexpr XY hpbarSize{ 256.f * hpbarScale, 48 * hpbarScale };
		static constexpr XY hpbarPos{ Cfg::xy7m.x, Cfg::xy7m.y - hpbarSize.y };	// left bottom

		static constexpr xx::RGBA8 hpbarContentColor{ 0xc9, 0x50, 0x37, 0xff };
		static constexpr float hpbarContentBorderWidth{ 2 * hpbarTexScale };
		static constexpr XY hpbarContentPos{ hpbarPos.x + hpbarContentBorderWidth, hpbarPos.y + hpbarContentBorderWidth };	// left bottom
		static constexpr XY hpbarContentMaxSize{ hpbarSize - hpbarContentBorderWidth * 2 };
		static constexpr XY hpbarContentScale{ hpbarContentMaxSize / ResTpFrames::_size_pixel };

		Stage* stage{};
		xx::Scale9 s9;
		float hp{ 1 }, hpMax{ 2 };		// need change when update

		void Init(Stage* stage_);
		UI_HPBar& SetValue(float hp_, float hpMax_);
		void Draw();
	};

}
