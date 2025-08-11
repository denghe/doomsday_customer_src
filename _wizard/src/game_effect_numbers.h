#pragma once

namespace Game {

	struct Scene;
	struct EffectText {
		constexpr static float cMoveDurationSeconds{ 0.5f };
		constexpr static float cMoveSpeedMin{ 50 / Cfg::fps };
		constexpr static float cMoveSpeedMax{ 100 / Cfg::fps };

		constexpr static float cFadeOutDurationSeconds{ 0.2f };
		constexpr static float cFadeOutStep = 1.f / (cFadeOutDurationSeconds / Cfg::frameDelay);

		QuadInstanceNumbersData data;

		int32_t lineNumber{};
		xx::RGBA8 color{};
		XY pos{}, inc{};
		int32_t timeout{};
		float alpha{};
		float scale{};

		// pos: original position,  dist: determine move direction
		void Init(XY const& pos_, XY const& vec_, xx::RGBA8 color_, float scale_, int32_t value_, bool includeSignal = false);

		int32_t Update(Scene *scene);

		void Draw(Scene* scene);
	};
}

namespace xx {
	template<>
	struct IsPod<Game::EffectText, void> : std::true_type {};
}

namespace Game {
	struct EffectTextManager {
		Scene* scene{};
		xx::Queue<EffectText> ens;

		void Init(Scene* scene_, int32_t cap);
		void Add(XY const& pos_, XY const& vec_, xx::RGBA8 color_, float scale_, int32_t value_, bool showSignal = false);
		bool Update();
		void Draw();
		void Clear();
	};
}
