#pragma once

namespace Game {

	struct Player_3 : Player_1 {
		void Init(Stage* stage_);
		int32_t Update() override;
	};
}
