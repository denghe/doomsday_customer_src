#pragma once

namespace Game {

	void Monster_Instantnoodles::Init(Stage* stage_, StatCfg const& statCfg_, XY const& pos_) {
		stage = stage_;
		frame = gLooper.res.monster_instantnoodles;

		pos = pos_;
		scale = { 1,1 };
		radius = frame->spriteSize.x * 0.5f;
		radians = 0;

		statCfg = statCfg_;
		StatCalc();
		healthPoint = sp.healthPoint * sp.healthRatio;

		whiteColorEndTime = 0;
		destroyTime = stage->time + (int32_t)Cfg::fps * 60 * 5;

		tarOffsetRadius = frame->spriteSize.x * 3;
		tarOffset = stage->GetRndPosDoughnut(tarOffsetRadius, 0.1f);
	}

}
