#pragma once

namespace Game {

	struct Player_1 : Player {
		void Init(Stage* ownerStage_);
		int32_t Update() override;
		void Draw() override;
	};

}
