#include "pch.h"
#include "looper.h"
#include "game_base.hpp"
#include "game_effect_death.hpp"
#include "game_main_menu.hpp"
#include "game_ground.hpp"
#include "game_player.hpp"
#include "game_player_bullet.hpp"
#include "game_player_skill_cfg.hpp"
#include "game_player_skill.hpp"
#include "game_monster.hpp"
#include "game_monstergen_generic.hpp"
#include "game_stage1.hpp"
#include "game_stage2.hpp"
#include "game_stage3.hpp"
#include "game_stage4.hpp"
#include "game_stage5.hpp"
#include "game_effect_text.hpp"
#include "game_monster_chips.hpp"
#include "game_monster_hamburger.hpp"
#include "game_monster_cola.hpp"
#include "game_monster_instantnoodles.hpp"
#include "game_monster_laptop.hpp"
#include "game_monster_roastduck.hpp"
#include "game_monster_sofa.hpp"
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
	btnCfg_Scale5.center = { 30, 30, (uint16_t)ResTpFrames::_size_ui_button.x - 30 * 2, (uint16_t)ResTpFrames::_size_ui_button.y - 30 * 2 };
	btnCfg_Scale5.color = xx::RGBA8_White;
	btnCfg_Scale5.txtColor = xx::RGBA8_Black;
	btnCfg_Scale5.txtPadding = { 50, 8 };
	btnCfg_Scale5.txtPaddingRightBottom = { 50, 30 };
	btnCfg_Scale5.txtScale = 5;

	btnCfg_Scale5Icon = btnCfg_Scale5;
	btnCfg_Scale5Icon.txtPadding = { 200, 8 };
	btnCfg_Scale5Icon.iconPadding = 15;

	btnCfg_Scale1Icon = btnCfg_Scale5Icon;
	btnCfg_Scale1Icon.borderScale = 0.5f;
	btnCfg_Scale1Icon.txtScale = 1;
	btnCfg_Scale1Icon.txtPadding = { 40, 2 };
	btnCfg_Scale1Icon.txtPaddingRightBottom = { 10, 6 };
	btnCfg_Scale1Icon.iconPadding = 6;

	clearColor = { 33, 33, 33, 255 };

	scene.Emplace<Game::MainMenu>()->Init();
}
