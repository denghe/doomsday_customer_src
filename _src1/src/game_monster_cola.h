#pragma once

namespace Game {

	struct Monster_Cola : Monster {
		static constexpr float cAimRange{ 10.f };

		void Init(Stage* stage_, StatCfg const& statCfg_, XY const& pos_);
		int32_t Update() override;
	};

}
