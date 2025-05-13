#pragma once

namespace Game {

	struct Player_1 : Player {
		xx::Shared<Skill_Shoot_1> shoot;
		xx::Shared<Skill_Control> control;

		void Init(Stage* stage_);
		void UpdateDamage(float damage) override;
		void UpdateAttackSpeed(float attackSpeed) override;
		int32_t Update() override;
	};

}
