#pragma once

namespace Game {


	struct Monster_1 : Monster {
		XY tarOffset{};
		float tarOffsetRadius{};

		int32_t Hurt(float dmg, XY const& d) override;
		void Init(Stage* stage_, XY const& pos_);
		int32_t Update() override;
		void Draw() override;
	};

}
