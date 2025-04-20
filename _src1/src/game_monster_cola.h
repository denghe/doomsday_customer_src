#pragma once

namespace Game {

	struct Monster_Cola : Monster {
		void Init(Stage* stage_, StatCfg const& statCfg_, XY const& pos_);
		int32_t Update() override;
	};

}
