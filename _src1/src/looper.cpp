#include "pch.h"
#include "looper.h"

#include "game_buff.hpp"
#include "game_base_creature.hpp"
#include "game_base_drawable.hpp"
#include "game_base_stageitem.hpp"
#include "game_base_monster.hpp"
#include "game_base_monstergen.hpp"
#include "game_base_stage.hpp"
#include "game_effect_death.hpp"
#include "game_warning_dash.hpp"
#include "game_main_menu.hpp"
#include "game_ground.hpp"
#include "game_player_1.hpp"
#include "game_player_2.hpp"
#include "game_player_3.hpp"
#include "game_bullet_1.hpp"
#include "game_bullet_2.hpp"
#include "game_skill_shoot_1.hpp"
#include "game_skill_shoot_2.hpp"
#include "game_skill_control.hpp"
#include "game_skill_dash.hpp"
#include "game_skill_move.hpp"
#include "game_monster.hpp"
#include "game_monstergen_generic.hpp"
#include "game_stage1.hpp"
#include "game_stage_dash.hpp"
#include "game_effect_text.hpp"
#include "game_monster_chips.hpp"
#include "game_monster_hamburger.hpp"
#include "game_monster_cola.hpp"
#include "game_monster_instantnoodles.hpp"
#include "game_monster_laptop.hpp"
#include "game_monster_roastduck.hpp"
#include "game_monster_sofa.hpp"
#include "game_spawner.hpp"
#include "game_ui_hpbar.hpp"
#include "game_ui_coinbar.hpp"
#include "game_ui_pause.hpp"
#include "game_ui_shop.hpp"
// more include here ...
#include "_test1.hpp"
#include "_test2.hpp"
#include "_test3.hpp"
#include "_test4.hpp"
#include "_test4.hpp"
#include "_test5.hpp"

Looper gLooper;

#ifdef __EMSCRIPTEN__
int32_t main() {
	gLooper.EngineInit();
	emscripten_request_animation_frame_loop([](double ms, void*)->EM_BOOL {
		return gLooper.JsLoopCallback(ms);
		}, nullptr);
}
#else
int32_t main() {
#ifdef WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif
	gLooper.showFps = true;
	gLooper.title = "doomsday_customer_src1";
	gLooper.Init();
	gLooper.Run<3>();
}
#endif

xx::Task<> Looper::MainTask() {
	co_await res.AsyncLoad("res/");

	rdd.Init(50, 128);

	btnCfg.frame = res.button;
	btnCfg.center = { 2, 3, 2, 2 };

	btnCfg1.frame = res.ui_button;
	btnCfg1.texScale = 1;
	btnCfg1.center = { 30, 30, (uint16_t)ResTpFrames::_size_ui_button.x - 30 * 2, (uint16_t)ResTpFrames::_size_ui_button.y - 30 * 2 };
	btnCfg1.color = xx::RGBA8_White;
	btnCfg1.txtColor = xx::RGBA8_Black;
	btnCfg1.txtPadding = { 15, 10 };
	btnCfg1.txtPaddingRightBottom = { 15, 10 };
	btnCfg1.txtScale = 1;
	btnCfg1.borderScale = 0.5f;

	btnCfg2.frame = res.ui_button;
	btnCfg2.texScale = 1;
	btnCfg2.center = { 30, 30, (uint16_t)ResTpFrames::_size_ui_button.x - 30 * 2, (uint16_t)ResTpFrames::_size_ui_button.y - 30 * 2 };
	btnCfg2.color = xx::RGBA8_White;
	btnCfg2.txtColor = xx::RGBA8_Black;
	btnCfg2.txtPadding = { 30, 20 };
	btnCfg2.txtPaddingRightBottom = { 30, 20 };
	btnCfg2.txtScale = 1;
	btnCfg2.borderScale = 0.5f;

	btnCfg2i = btnCfg2;
	btnCfg2i.txtPadding = { 80, 20 };
	btnCfg2i.iconPadding = 12;

	btnCfg3.frame = res.ui_button;
	btnCfg3.texScale = 1;
	btnCfg3.center = { 30, 30, (uint16_t)ResTpFrames::_size_ui_button.x - 30 * 2, (uint16_t)ResTpFrames::_size_ui_button.y - 30 * 2 };
	btnCfg3.color = xx::RGBA8_White;
	btnCfg3.txtColor = xx::RGBA8_Black;
	btnCfg3.txtPadding = { 40, 30 };
	btnCfg3.txtPaddingRightBottom = { 40, 30 };
	btnCfg3.txtScale = 2;
	btnCfg3.borderScale = 1.f;
	btnCfg3.iconPadding = 20;

	btnCfg3i = btnCfg3;
	btnCfg3i.txtPadding = { 120, 30 };


	btnCfg4.frame = res.ui_button;
	btnCfg4.texScale = 1;
	btnCfg4.center = { 30, 30, (uint16_t)ResTpFrames::_size_ui_button.x - 30 * 2, (uint16_t)ResTpFrames::_size_ui_button.y - 30 * 2 };
	btnCfg4.color = xx::RGBA8_White;
	btnCfg4.txtColor = xx::RGBA8_Black;
	btnCfg4.txtPadding = { 40, 5 };
	btnCfg4.txtPaddingRightBottom = { 40, 20 };
	btnCfg4.txtScale = 4;

	clearColor = { 0xdf, 0xc1, 0x98, 255 };

	scene.Emplace<Game::MainMenu>()->Init();
}
