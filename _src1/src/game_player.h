#pragma once

namespace Game {

	struct Player_1 : Player {
		void Init(Stage* stage_);
		int32_t Update() override;
		void Draw() override;
	};

	struct Player_2 : Player_1 {
		bool needFlipX{};
		void Init(Stage* stage_);
		int32_t Update() override;
		void Draw() override;
	};

}
