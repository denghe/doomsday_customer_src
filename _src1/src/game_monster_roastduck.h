﻿#pragma once

namespace Game {

	struct Monster_Roastduck : Monster {
		void Init(Stage* stage_, StatCfg const& statCfg_, XY const& pos_);
		int32_t Update() override;
	};

}
