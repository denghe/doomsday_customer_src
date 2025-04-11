#include "pch.h"
#include "looper.h"

#include "game_base_creature.hpp"
#include "game_base_drawable.hpp"
#include "game_base_monster.hpp"
#include "game_base_monstergen.hpp"
#include "game_base_stage.hpp"
#include "game_effect_death.hpp"
#include "game_main_menu.hpp"
#include "game_ground.hpp"
#include "game_player_1.hpp"
#include "game_player_2.hpp"
#include "game_player_3.hpp"
#include "game_bullet_1.hpp"
#include "game_skill_shoot_1.hpp"
#include "game_skill_control.hpp"
#include "game_skill_dash.hpp"
#include "game_skill_move.hpp"
#include "game_monster.hpp"
#include "game_monstergen_generic.hpp"
#include "game_stage1.hpp"
#include "game_stage2.hpp"
#include "game_stage3.hpp"
#include "game_stage4.hpp"
#include "game_stage5.hpp"
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
#include "game_ui_pause.hpp"
#include "game_ui_shop.hpp"
// more include here ...
#include "_test1.hpp"

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

	btnCfg_Scale5.frame = res.ui_button;
	btnCfg_Scale5.texScale = 1;
	btnCfg_Scale5.center = { 30, 30, (uint16_t)ResTpFrames::_size_ui_button.x - 30 * 2, (uint16_t)ResTpFrames::_size_ui_button.y - 30 * 2 };
	btnCfg_Scale5.color = xx::RGBA8_White;
	btnCfg_Scale5.txtColor = xx::RGBA8_Black;
	btnCfg_Scale5.txtPadding = { 50, 8 };
	btnCfg_Scale5.txtPaddingRightBottom = { 50, 30 };
	btnCfg_Scale5.txtScale = 5;

	btnCfg_Scale4.frame = res.ui_button;
	btnCfg_Scale4.texScale = 1;
	btnCfg_Scale4.center = { 30, 30, (uint16_t)ResTpFrames::_size_ui_button.x - 30 * 2, (uint16_t)ResTpFrames::_size_ui_button.y - 30 * 2 };
	btnCfg_Scale4.color = xx::RGBA8_White;
	btnCfg_Scale4.txtColor = xx::RGBA8_Black;
	btnCfg_Scale4.txtPadding = { 40, 5 };
	btnCfg_Scale4.txtPaddingRightBottom = { 40, 20 };
	btnCfg_Scale4.txtScale = 4;

	btnCfg_Scale3.frame = res.ui_button;
	btnCfg_Scale3.texScale = 1;
	btnCfg_Scale3.center = { 30, 30, (uint16_t)ResTpFrames::_size_ui_button.x - 30 * 2, (uint16_t)ResTpFrames::_size_ui_button.y - 30 * 2 };
	btnCfg_Scale3.color = xx::RGBA8_White;
	btnCfg_Scale3.txtColor = xx::RGBA8_Black;
	btnCfg_Scale3.txtPadding = { 30, 0 };
	btnCfg_Scale3.txtPaddingRightBottom = { 30, 10 };
	btnCfg_Scale3.txtScale = 3;

	btnCfg_Scale2.frame = res.ui_button;
	btnCfg_Scale2.texScale = 0.5;
	btnCfg_Scale2.center = { 30, 30, (uint16_t)ResTpFrames::_size_ui_button.x - 30 * 2, (uint16_t)ResTpFrames::_size_ui_button.y - 30 * 2 };
	btnCfg_Scale2.color = xx::RGBA8_White;
	btnCfg_Scale2.txtColor = xx::RGBA8_Black;
	btnCfg_Scale2.txtPadding = { 40, 20 };
	btnCfg_Scale2.txtPaddingRightBottom = { 40, 20 };
	btnCfg_Scale2.txtScale = 1;

	btnCfg_Scale2Icon.frame = res.ui_button;
	btnCfg_Scale2Icon.texScale = 1;
	btnCfg_Scale2Icon.center = { 30, 30, (uint16_t)ResTpFrames::_size_ui_button.x - 30 * 2, (uint16_t)ResTpFrames::_size_ui_button.y - 30 * 2 };
	btnCfg_Scale2Icon.color = xx::RGBA8_White;
	btnCfg_Scale2Icon.txtColor = xx::RGBA8_Black;
	btnCfg_Scale2Icon.txtPadding = { 80, 20 };
	btnCfg_Scale2Icon.txtPaddingRightBottom = { 30, 20 };
	btnCfg_Scale2Icon.txtScale = 1;
	btnCfg_Scale2Icon.borderScale = 0.5f;
	btnCfg_Scale2Icon.iconPadding = 12;


	clearColor = { 33, 33, 33, 255 };

	scene.Emplace<Game::MainMenu>()->Init();
}
