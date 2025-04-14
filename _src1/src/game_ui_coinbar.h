#pragma once

namespace Game {

	struct Stage;
	struct UI_CoinBar {
		// coin bar arguments
		static constexpr float coinBarTopMargin{ 10.f / 1080.f * Cfg::height };
		static constexpr XY coinIconSize{ 64.f / 1080.f * Cfg::height };
		static constexpr XY coinIconScale{ coinIconSize / ResTpFrames::_size_ui_money };
		static constexpr float coinIconTextPadding{ 10.f / 1080.f * Cfg::height };
		static constexpr float coinTextRightMargin{ (64.f + 10.f) / 1080.f * Cfg::height };	// for pause button
		static constexpr float coinCharWidth{ 28.f / 1080.f * Cfg::height };
		static constexpr float coinCharScaleX{ coinCharWidth / ResTpFrames::_size_font_outline_48.x };
		static constexpr float coinCharScaleY{ coinCharScaleX * 1.2 };
		static constexpr float coinCharPadding{ 2.f / 1080.f * Cfg::height };
		static constexpr float coinCharMaxCount{ 8 };
		static constexpr float coinTextWidth{ coinCharWidth * coinCharMaxCount + coinCharPadding * (coinCharMaxCount - 1)};
		static constexpr XY coinIconPos{ Cfg::xy9m.x - coinTextWidth - coinIconTextPadding - coinTextRightMargin - coinIconSize.x, Cfg::xy9.y - coinIconSize.y / 2 - coinBarTopMargin };
		static constexpr XY coinIconAnchor{ 0, 0.5 };
		static constexpr XY coinTextPos{ Cfg::xy9m.x - coinTextWidth - coinTextRightMargin, coinIconPos.y };
		static constexpr XY coinTextAnchor{ 0, 0.5 };

		Stage* stage{};
		std::array<char, 12> buf{};			// value to string cache
		int32_t len{};						// buf's len
		double coin{};						// need change when update

		void Init(Stage* stage_);
		UI_CoinBar& SetValue(double coin_);
		void Draw();
	};

}
