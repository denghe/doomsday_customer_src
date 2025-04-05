#pragma once

namespace Game {

	struct Skill_1 : Skill {
		void Init(Creature* creature_, SkillCfg* skillCfg_);
		int32_t Update() override;
	};

	struct Skill_DashAttack : Skill {
		static constexpr int32_t coolDown{ (int32_t)Cfg::fps * 3 };
		int32_t coolDownTimer{};

		void Init(Creature* creature, SkillCfg* skillCfg_);
		int32_t Update() override;
	};
}
