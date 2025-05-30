#pragma once

namespace Game {

	struct Monster : Drawable {
		static constexpr float cMoveSpeed{ 500.f / Cfg::fps };

		int32_t indexAtItems{ -1 }, indexAtCells{ -1 };
		Monster* prev{}, * next{};

		xx::Ref<xx::Frame> frame;
		xx::Weak<MonsterFormation> monsterFormation;
		int32_t monsterFormationPosIdx{};
		float radius{};
		float radians{};
		XY lightRadius{};

		void Init(Stage* stage_, xx::Ref<xx::Frame> frame_, int32_t bornPosIdx
			, xx::Weak<MonsterFormation> monsterFormation_, int32_t monsterFormationPosIdx_);
		int32_t Update() override;
		void Draw() override;
	};

}
