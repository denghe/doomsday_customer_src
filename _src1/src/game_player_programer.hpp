#pragma once

namespace Game {

	inline void Player_Programmer::Init(Stage* stage_) {
		stage = stage_;
		creatureType = CreatureTypes::Player_Programmer;
		frame = gLooper.res.player1;
		collectRange = 1000;

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

	inline void Player_Programmer::UpdateDamage(float damage) {
		shoot->damage += damage;
		if (shoot->damage < 0.f) {
			shoot->damage = 0.f;
		}
		xx::CoutN("damage: ", shoot->damage);
	}

	inline void Player_Programmer::UpdateAttackSpeed(float attackSpeed) {
		shoot->shootSpeed *= (1.f + attackSpeed);
		xx::CoutN("attackSpeed: ", shoot->shootSpeed);
	}

	inline void Player_Programmer::UpdateHealthPoint(int32_t hp) {
		healthPoint += hp;
		if (healthPoint > healthPointMax) {
			healthPoint = healthPointMax;
		}
		if (healthPoint < 0) {
			healthPoint = 0;
		}
		xx::CoutN("healthPoint: ", healthPoint);
	}

	inline int32_t Player_Programmer::Update() {
		shoot->Update();
		control->Update();

		if (auto o = stage->loots.FindNearestByRange(pos.x, pos.y, collectRange)) {
			auto loot = xx::SharedFromThis(o);
			stage->loots.Remove(o);
			stage->flyingLoots.Add(loot);
		}

		return 0;
	}
}
