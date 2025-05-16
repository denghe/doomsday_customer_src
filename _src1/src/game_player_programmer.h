#pragma once

namespace Game {

	struct Player_Programmer : Player {

		xx::Shared<Skill_Shoot_1> shoot;
		xx::Shared<Skill_Control> control;

		void Init(Stage* stage_);
		void UpdateDamage(float damage) override;
		void UpdateAttackSpeed(float attackSpeed) override;
		void UpdateHealthPoint(int32_t hp) override;

		int32_t Update() override;
	};

}
