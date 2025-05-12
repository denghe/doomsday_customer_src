#pragma once

namespace Game {

	inline void Player_1::Init(Stage* stage_) {
		stage = stage_;
		creatureType = CreatureTypes::Player_Programer;
		frame = gLooper.res.player1;

		pos = stage_->GetPlayerBornPos();
		scale = { 1,1 };
		offsetRatio = { 0.f, -0.3f };
		radius = ResTpFrames::_size_player1.x * 0.5f;
		radians = 0;

		// todo:
		// statCfg.init.xxxxxx = ?????
		StatCalc();
		healthPoint = healthPointMax;

		// add init skill
		shoot = skills.Emplace().Emplace<Skill_Shoot_1>();
		shoot->Init(this);

		control = skills.Emplace().Emplace<Skill_Control>();
		control->Init(this);

		// init buffs
		buffs.Init(this);
	}

	inline void Player_1::UpdateDamage(float damage) {
		shoot->damage += damage;
		if (shoot->damage < 0.f) {
			shoot->damage = 0.f;
		}
	}

	inline void Player_1::UpdateAttackSpeed(float attackSpeed) {
		shoot->shootSpeed *= (1.f + attackSpeed);
	}

	inline int32_t Player_1::Update() {
		shoot->Update();
		control->Update();

		if (auto o = stage->loots.FindNearestByRange(pos.x,pos.y,collectRange)) {
			// todo loot move to Player
		}
		return 0;
	}

}
