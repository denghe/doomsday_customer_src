#pragma once

namespace Game {

	void Monster_Laptop::Init(Stage* stage_, XY const& pos_) {
		creatureType = CreatureTypes::Monster_Laptop;
		stage = stage_;
		frame = gLooper.res.monster_laptop;

		pos = pos_;
		scale = { 1,1 };
		radius = frame->spriteSize.x * 0.5f;
		radians = 0;

		// todo: fill statCfg
		healthPointMax = Stat_t(stage->roundId * 20 + 100);
		StatCalc();
		healthPoint = healthPointMax;

		whiteColorEndTime = 0;
		destroyTime = stage->time + (int32_t)Cfg::fps * 60 * 5;

		tarOffsetRadius = frame->spriteSize.x * 3;
		tarOffset = stage->GetRndPosDoughnut(tarOffsetRadius, 0.1f);
	}

}
