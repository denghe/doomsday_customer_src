﻿#pragma once

namespace Game {


	struct Monster_Chips : Monster {
		XY tarOffset{};
		float tarOffsetRadius{};

		void Init(Stage* stage_, XY const& pos_);
		int32_t Update() override;
	};

}
