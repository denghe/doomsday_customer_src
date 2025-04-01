#pragma once

namespace Game {

	struct Skill_1 : Skill {
		void Init(Player* player_, SkillCfg* skillCfg_);
		int32_t Update() override;
	};

}
