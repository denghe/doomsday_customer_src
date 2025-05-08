#pragma once

namespace Game {

	void Monster_Cola::Init(Stage* stage_, XY const& pos_) {
		stage = stage_;
		frame = gLooper.res.monster_cola;
		state = State::Idle;

		pos = pos_;
		scale = { 1,1 };
		radius = frame->spriteSize.x * 0.5f;
		radians = 0;

		// todo: fill statCfg
		healthPointMax = stage->roundId * 8 + 30;
		StatCalc();
		healthPoint = healthPointMax;

		whiteColorEndTime = 0;
		destroyTime = stage->time + (int32_t)Cfg::fps * 60 * 5;

		tarOffsetRadius = frame->spriteSize.x * 3;
		tarOffset = stage->GetRndPosDoughnut(tarOffsetRadius, 0.1f);

		auto bulletRadius = gLooper.res._size_monster_hamburger.x*.5f;
		auto aimRange = cAimRange * bulletRadius;

		skills.Emplace().Emplace<Skill_MoveToPlayer>()->Init(this, aimRange);
		skills.Emplace().Emplace<Skill_Shoot_2>()->Init(this, aimRange);
		SetName({ 0, -100 }, U"可乐怪");
	}

	int32_t Monster_Cola::Update() {
		// auto use skill
		for (auto i = skills.len - 1; i >= 0; --i) {
			auto& skill = skills[i];
			if (skill->Update()) {
				skills.SwapRemoveAt(i);
			}
		}
		Idle();
		return destroyTime <= stage->time;
	}

	void Monster_Cola::Rewards(Creature* target) {
		target->coin += 5;
		healthPoint += 10;
		if (healthPoint > healthPointMax) {
			healthPoint = healthPointMax;
		}
	}
}
