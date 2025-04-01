#pragma once

namespace Game {


	struct Monster_1 : Monster {
		void Init(Stage* stage_, XY const& pos_);
		int32_t Update() override;
		void Draw() override;
	};

}
