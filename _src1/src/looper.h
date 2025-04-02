﻿#pragma once
#include "pch.h"
#include "res_tp_frames.h"

struct Cfg : xx::GDesign<1920, 1080, 120> {
	static constexpr XY designSize{ 3840, 2160 };
	static constexpr float defaultScale{ 0.5f };
	static constexpr float unitSize{ 128.f };
	static constexpr XYi gridSize{ 2000, 2000 };			// default value
	static constexpr int32_t numMaxMonsters{ 100000 };
};

#include "game_space.h"
#include "game_effect_text.h"
#include "game_base.h"
#include "game_main_menu.h"
#include "game_ground.h"
#include "game_player_skill_cfg.h"
#include "game_player_skill.h"
#include "game_player_bullet.h"
#include "game_player.h"
#include "game_monster.h"
#include "game_monster_generator.h"
#include "game_stage1.h"
// more include here ...

struct Looper : xx::Engine<Looper>, Cfg {
	xx::Task<> MainTask();

	ResTpFrames res;
	xx::Scale9SpriteConfig btnCfg, btnCfg2;
	xx::SpaceGridRingDiffuseData rdd;
	xx::Listi32<std::pair<float, Game::Drawable*>> yDraws;
};

extern Looper gLooper;
