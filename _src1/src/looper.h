#pragma once
#include "pch.h"
#include "res_tp_frames.h"

struct Cfg : xx::GDesign<1920, 1080, 120> {
	static constexpr XY designSize{ 3840, 2160 };
	static constexpr float unitSize{ 128.f };
	static constexpr float unitRadius{ unitSize / 2 };
	static constexpr float defaultScale{ height / 20 / unitSize };
	static constexpr XYi gridSize{ 60, 60 };			// default value
	static constexpr int32_t numMaxMonsters{ 50000 };
};

#include "xx2d_camera_ex.h"
#include "game_shader_numbers.h"
#include "game_space.h"
#include "game_effect_text.h"
#include "game_stat.h"
#include "game_buff.h"
#include "game_base.h"
#include "game_effect_death.h"
#include "game_warning_dash.h"
#include "game_main_menu.h"
#include "game_ground.h"
#include "game_skill_shoot_1.h"
#include "game_skill_shoot_2.h"
#include "game_skill_control.h"
#include "game_skill_move.h"
#include "game_skill_dash.h"
#include "game_skill_attack.h"
#include "game_bullet_1.h"
#include "game_bullet_2.h"
#include "game_player_1.h"
#include "game_player_2.h"
#include "game_player_3.h"
#include "game_player_programer.h"
#include "game_monster.h"
#include "game_monstergen_generic.h"
#include "game_ui_hpbar.h"
#include "game_ui_coinbar.h"
#include "game_ui_shop.h"
#include "game_ui_pause.h"
#include "game_stage1.h"
#include "game_stage_dash.h"
#include "game_monster_chips.h"
#include "game_monster_hamburger.h"
#include "game_monster_cola.h"
#include "game_monster_instantnoodles.h"
#include "game_monster_laptop.h"
#include "game_monster_roastduck.h"
#include "game_monster_sofa.h"
#include "game_monster_house.h"
#include "game_spawner.h"
#include "game_env_grass.h"
// more include here ...
#include "_test1.h"
#include "_test2.h"
#include "_test3.h"
#include "_test4.h"
#include "_test5.h"
#include "_test6.h"
#include "_test7.h"
#include "_test8.h"



struct Looper : xx::Engine<Looper>, Cfg {
	xx::Task<> MainTask();
#ifndef __EMSCRIPTEN__
	BS::thread_pool<> threadPool;
#endif

    Game::Shader_QuadInstanceNumbers shaderNumbers;

	xx::Ref<xx::GLTexture> nums1;	// res/nums1.png
	ResTpFrames res;
	xx::Scale9SpriteConfig btnCfg, btnCfg1, btnCfg2, btnCfg2i, btnCfg3, btnCfg3i, btnCfg4, buffInfoPanelCfg;
	xx::SpaceGridRingDiffuseData rdd;
	xx::Listi32<std::pair<float, Game::Drawable*>> yDraws;
};

extern Looper gLooper;
