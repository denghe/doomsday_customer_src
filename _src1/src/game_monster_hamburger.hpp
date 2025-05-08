#pragma once

namespace Game {

	void Monster_Hamburger::Init(Stage* stage_, XY const& pos_) {
		stage = stage_;
		frame = gLooper.res.monster_hamburger;

		pos = pos_;
		scale = { 1,1 };
		radius = frame->spriteSize.x * 0.5f;
		radians = 0;

		// todo: fill statCfg
		healthPointMax = stage->roundId * 5 + 15;
		StatCalc();
		healthPoint = healthPointMax;

		whiteColorEndTime = 0;
		destroyTime = stage->time + (int32_t)Cfg::fps * 60 * 5;

		tarOffsetRadius = frame->spriteSize.x * 3;
		tarOffset = stage->GetRndPosDoughnut(tarOffsetRadius, 0.1f);
	}

	void Monster_Hamburger::Rewards(Creature* target) {
		target->coin += 5;
		healthPoint += 10;
		if (healthPoint > healthPointMax) {
			healthPoint = healthPointMax;
		}
	}
}
