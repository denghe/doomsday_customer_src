#pragma once

namespace Game {

	struct Spawner : Drawable {
		std::function<void(Stage* stage_, XY const& pos_)> onDispose;
		int32_t elapsedTime{}, flashTime{};
		bool visible{};
		int32_t n1{};
		template<typename F> void Init(Stage* stage_, XY const& pos_, float scale_, float delaySeconds, F&& createFunc);
		int32_t Update() override;
		void Draw() override;
	};

}
