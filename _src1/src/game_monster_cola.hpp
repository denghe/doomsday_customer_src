#pragma once

namespace Game {

	void Monster_Cola::Init(Stage* stage_, StatCfg const& statCfg_, XY const& pos_) {
		stage = stage_;
		frame = gLooper.res.monster_cola;

		pos = pos_;
		scale = { 1,1 };
		radius = frame->spriteSize.x * 0.5f;
		radians = 0;

		statCfg = statCfg_;
		StatCalc();
		healthPoint = healthPointMax;

		whiteColorEndTime = 0;
		destroyTime = stage->time + (int32_t)Cfg::fps * 60 * 5;

		tarOffsetRadius = frame->spriteSize.x * 3;
		tarOffset = stage->GetRndPosDoughnut(tarOffsetRadius, 0.1f);

		skills.Emplace().Emplace<Skill_Shoot_2>()->Init(this);
		skills.Emplace().Emplace<Skill_MoveToPlayer>()->Init(this);
		SetName({ 0, -100 }, U"可乐怪");
	}

}
