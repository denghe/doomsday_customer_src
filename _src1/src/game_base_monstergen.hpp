﻿#pragma once

namespace Game {

	inline StatPanel& MonsterGen::Init(Stage* stage_, int activeTime_, int destroyTime_, float generateNumsPerSeconds_) {
		stage = stage_;
		activeTime = activeTime_;
		destroyTime = destroyTime_;
		countIncPerFrame = generateNumsPerSeconds_ / Cfg::fps;
		return statCfg.init;
	}

	template<typename T>
	inline void MonsterGen::CreateFromRect() {
		auto& p = stage->player->pos;
		auto& rnd = stage->rnd;
		static constexpr auto ds_2 = Cfg::designSize / 2;
		for (countPool += countIncPerFrame; countPool >= 1; --countPool) {
			if (stage->monsters.items.len < Cfg::numMaxMonsters) {
				XY pos{};
				switch (stage->rnd.Next<int32_t>(0, 4)) {
				case 0:
					pos = { p.x - ds_2.x - Cfg::unitSize
						, p.y + rnd.Next<float>(-ds_2.y - Cfg::unitSize, ds_2.y + Cfg::unitSize) };
					break;
				case 1:
					pos = { p.x + ds_2.x + Cfg::unitSize
						, p.y + rnd.Next<float>(-ds_2.y - Cfg::unitSize, ds_2.y + Cfg::unitSize) };
					break;
				case 2:
					pos = { p.x + rnd.Next<float>(-ds_2.x - Cfg::unitSize, ds_2.x + Cfg::unitSize)
						, p.y - ds_2.y - Cfg::unitSize };
					break;
				case 3:
					pos = { p.x + rnd.Next<float>(-ds_2.x - Cfg::unitSize, ds_2.x + Cfg::unitSize)
						, p.y + ds_2.y + Cfg::unitSize };
					break;
				}

				auto m = xx::MakeShared<T>();
				m->Init(stage, pos);
				stage->monsters.Add(std::move(m));
			}
		}
	}

	template<typename T>
	inline void MonsterGen::CreateFromDoughnut() {
		auto& p = stage->player->pos;
		auto& rnd = stage->rnd;
		for (countPool += countIncPerFrame; countPool >= 1; --countPool) {
			if (stage->monsters.items.len < Cfg::numMaxMonsters) {
				auto pos = stage->player->pos + stage->GetRndPosDoughnut(Cfg::designSize.x, 200.f);

				// todo: calculate scale
				stage->spawners.Emplace().Emplace()->Init(stage, statCfg, pos, 1.f, 1.5f, [](Stage* stage_, StatCfg const& statCfg_, XY const& pos_) {
					auto m = xx::MakeShared<T>();
					m->Init(stage_, statCfg_, pos_);
					stage_->monsters.Add(std::move(m));
				});
			}
		}
	}

}
