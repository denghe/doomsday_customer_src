#pragma once

namespace Game {

	struct PlayerSkill {
		// refs
		Player* ownerPlayer{};

		PlayerSkillCfg cfg;
		float shootCountPool{};	// runtime

		virtual int32_t Update() { return 0; };
		virtual ~PlayerSkill() {};
	};

	struct PlayerSkill_1 : PlayerSkill {
		void Init(Player* ownerPlayer_);
		int32_t Update() override;
	};
}
