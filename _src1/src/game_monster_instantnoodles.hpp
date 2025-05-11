#pragma once

namespace Game {

	void Monster_Instantnoodles::Init(Stage* stage_, XY const& pos_) {
		stage = stage_;
		frame = gLooper.res.monster_instantnoodles;

		pos = pos_;
		scale = { 1,1 };
		radius = frame->spriteSize.x * 0.5f;
		radians = 0;

		// todo: fill statCfg
		healthPointMax = Stat_t(100 * stage->roundId + 1500);
		StatCalc();
		healthPoint = healthPointMax;

		whiteColorEndTime = 0;
		destroyTime = stage->time + (int32_t)Cfg::fps * 60 * 5;

		tarOffsetRadius = frame->spriteSize.x * 3;
		tarOffset = stage->GetRndPosDoughnut(tarOffsetRadius, 0.1f);
	}

	void Monster_Instantnoodles::Rewards(Creature* target) {
		target->coin += 50;
		healthPoint += 400;
		if (healthPoint > healthPointMax) {
			healthPoint = healthPointMax;
		}

		// todo attackSpeed damage
	}
}
