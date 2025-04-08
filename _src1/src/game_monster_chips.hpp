#pragma once

namespace Game {

	void Monster_Chips::Init(Stage* stage_, StatCfg const& statCfg_, XY const& pos_) {
		stage = stage_;
		frame = gLooper.res.monster_chips;
		state = State::Idle;

		pos = pos_;
		scale = { 1,1 };
		radius = frame->spriteSize.x * 0.5f;
		radians = 0;

		statCfg = statCfg_;
		StatCalc();
		healthPoint = sp.healthPoint;

		whiteColorEndTime = 0;
		destroyTime = stage->time + (int32_t)Cfg::fps * 60 * 5;

		tarOffsetRadius = frame->spriteSize.x * 3;
		tarOffset = stage->GetRndPosDoughnut(tarOffsetRadius, 0.1f);

		skills.Emplace().Emplace<Skill_DashAttack>()->Init(this, stage->skillCfgs[1]);

		state = State::Idle;
	}

	int32_t Monster_Chips::Update() {
		// auto use skill
		for (auto i = skills.len - 1; i >= 0; --i) {
			auto& skill = skills[i];
			if (skill->Update()) {
				skills.SwapRemoveAt(i);
			}
		}

		auto p = stage->player.pointer;
		xx::XY pp{};
		xx::XYf d{};
		float dd{};
		float r2{};
		float mag{};
		xx::XYf norm{};
		switch (state)
		{
		case State::Idle:
			// move to player
			Idle();
			pp = p->pos;
			d = pp - pos;
			dd = d.x * d.x + d.y * d.y;
			r2 = p->radius + radius;
			if (dd < r2 * r2) {
				// cross with player?
				//p->Hurt(damage, d);   // todo
			}
			else {
				d = pp - pos + tarOffset;
				dd = d.x * d.x + d.y * d.y;
				if (dd < radius * radius) {
					// reached offset point? reset offset point
					tarOffset = stage->GetRndPosDoughnut(tarOffsetRadius, 0.1f);
				}
				// calc & move
#if 0
				// slowly than mag normalize
				auto r = std::atan2f(d.y, d.x);
				pos += XY{ std::cosf(r) * moveSpeed, std::sinf(r) * moveSpeed };
#else
				// faster than atan2 + sin cos  1/4
				mag = std::sqrt(dd);
				norm = d / mag;
				pos += norm * movementSpeedPerFrame;
#endif
				stage->ForceLimit(pos);
				stage->monsters.Update(this);	// sync space index
			}
			break;
		case State::Dash:
			pp = p->pos;
			d = pp - pos;
			dd = d.x * d.x + d.y * d.y;
			r2 = p->radius + radius;
			if (dd < r2 * r2) {
				ExitDash();
			}
			else {
				mag = std::sqrt(dd);
				norm = d / mag;
				pos += norm * movementSpeedPerFrame;
				stage->ForceLimit(pos);
				stage->monsters.Update(this);	// sync space index
			}
			break;
		case State::Knockback:
			knockbackSpeed -= knockbackReduceValuePerFrame;
			if (knockbackSpeed <= 0) {
				ExitKnockback();
			}
			else {
				pos = pos + knockbackDist * knockbackSpeed;
				stage->ForceLimit(pos);
				stage->monsters.Update(this);	// sync space index
			}
			break;
		default:
			Idle();
			break;
		}

		// todo
		return destroyTime <= stage->time;
	}

	void Monster_Chips::ExitKnockback() {
		knockback = false;
		state = State::Idle;
	}

	void Monster_Chips::ExitDash() {
		state = State::Idle;
	}
}
