#pragma once

namespace Game {

	struct Monster_Chips : Monster {
		void Init(Stage* stage_, StatCfg const& statCfg_, XY const& pos_);
		int32_t Update() override;
		void ExitKnockback();
		void ExitDash();
	};

}
