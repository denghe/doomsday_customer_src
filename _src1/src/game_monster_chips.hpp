﻿#pragma once

namespace Game {

	void Monster_Chips::Init(Stage* stage_, XY const& pos_) {
		stage = stage_;
		frame = gLooper.res.monster_chips;

		pos = pos_;
		scale = { 1,1 };
		radius = frame->spriteSize.x * 0.5f;
		radians = 0;

		hp = 10;// 20.f + stage->round * 6;
		damage = 1;
		moveSpeed = 300.f / Cfg::fps;
		criticalRate = 0.1f;
		criticalDamageRatio = 2;

		whiteColorEndTime = 0;
		destroyTime = stage->time + (int32_t)Cfg::fps * 60 * 5;

		tarOffsetRadius = frame->spriteSize.x * 3;
		tarOffset = stage->GetRndPosDoughnut(tarOffsetRadius, 0.1f);

		skills.Emplace().Emplace<Skill_DashAttack>()->Init(this, stage->skillCfgs[1]);
	}

}
