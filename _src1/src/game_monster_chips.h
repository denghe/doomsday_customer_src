﻿#pragma once

namespace Game {

	struct Monster_Chips : Monster {
		void Init(Stage* stage_, XY const& pos_);
		int32_t Update() override;
		void Rewards() override;
		void Draw() override;
	};
}
