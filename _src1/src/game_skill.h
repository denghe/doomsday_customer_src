#pragma once

namespace Game {

	struct Skill_1 : ShootSkill {
		void Init(Creature* creature_, SkillCfg* skillCfg_);
		int32_t Update() override;
	};
}
