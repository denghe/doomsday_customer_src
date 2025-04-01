#pragma once

namespace Game {

	struct PlayerSkill_1 : Skill {
		void Init(Player* ownerPlayer_);
		int32_t Update() override;
	};

}
