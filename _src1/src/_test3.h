#pragma once

namespace Game {
	
	struct CreatureGroup : Drawable {
		static constexpr float cMaxDistance{ ResTpFrames::_size_monster_hamburger.x * 0.7f };
		static constexpr float cMinDistance{ cMaxDistance * 0.4f };
		xx::Listi32<XY> memberPoss;
		XY vec{};
		float distanceStep{};
		float distance{};
		int32_t n{};
		int32_t endLifeTime{};
		void Init(Stage* stage_, XY const& pos_, float radians_, float duration_, int32_t len);
		int32_t Update() override;
		void Draw() override;
	};

	struct Test3 : Stage {
		xx::Listi32<xx::Shared<CreatureGroup>> cgs;

		void Init() override;
		void Update() override;
		void DrawCustomOrderYItem(xx::Listi32<std::pair<float, Game::Drawable*>>& yd, XY areaMin, XY areaMax) override;
	};

}
