#pragma once

namespace Game {

	struct Player_2 : Player {
		void Init(Stage* stage_);
		int32_t Update() override;
	};
}
