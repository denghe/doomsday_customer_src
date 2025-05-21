#pragma once

namespace Game {

	struct Stage;
	struct EffectText {
		constexpr static float cMoveDurationSeconds{ 0.5f };
		constexpr static float cMoveSpeedMin{ 100 / Cfg::fps };
		constexpr static float cMoveSpeedMax{ 250 / Cfg::fps };

		constexpr static float cFadeOutDurationSeconds{ 0.2f };
		constexpr static float cFadeOutStep = 1.f / (cFadeOutDurationSeconds / Cfg::frameDelay);

		std::array<char, 12> buf{};			// value to string cache
		int32_t len{};						// buf's len

		int32_t lineNumber{};
		xx::RGBA8 color{};
		XY pos{}, inc{};
		int32_t timeout{};
		float alpha{};
		float scale{};

		// pos: original position,  dist: determine move direction
		void Init(XY const& pos_, XY const& vec_, xx::RGBA8 color_, float scale_, double value_);

		int32_t Update(Stage *stage);

		void Draw(Stage* stage);
	};
}

namespace xx {
	template<>
	struct IsPod<Game::EffectText, void> : std::true_type {};
}

namespace Game {
	struct EffectTextManager {
		Stage* stage{};
		xx::Queue<EffectText> ens;

		void Init(Stage* stage_, int32_t cap);
		void Add(XY const& pos_, XY const& vec_, xx::RGBA8 color_, float scale_, double value_);
		bool Update();
		void Draw();
		void Clear();
	};
}
