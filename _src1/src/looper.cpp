#include "pch.h"
#include "looper.h"
#include "game_base.hpp"
#include "game_main_menu.hpp"
#include "game_ground.hpp"
#include "game_player.hpp"
#include "game_player_bullet.hpp"
#include "game_player_skill_cfg.hpp"
#include "game_player_skill.hpp"
#include "game_monster.hpp"
#include "game_monster_generator.hpp"
#include "game_monster_generator_chips.hpp"
#include "game_monster_generator_hamburger.hpp"
#include "game_monster_generator_cola.hpp"
#include "game_monster_generator_instantnood.hpp"
#include "game_stage1.hpp"
#include "game_effect_text.hpp"
#include "game_monster_chips.hpp"
#include "game_monster_hamburger.hpp"
#include "game_monster_cola.hpp"
#include "game_monster_instantnoodles.hpp"

// more include here ...

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

	btnCfg2.frame = res.ui_button;
	btnCfg2.center = { 30, 25, (uint16_t)ResTpFrames::_size_ui_button_.x - 30 * 2, (uint16_t)ResTpFrames::_size_ui_button_.y - 25 * 2 };
	btnCfg2.color = xx::RGBA8_White;
	btnCfg2.txtColor = xx::RGBA8_Black;

	btnCfg3 = btnCfg2;
	btnCfg3.borderScale = 0.5f;
	btnCfg3.txtPadding = {};


	clearColor = { 33, 33, 33, 255 };

	scene.Emplace<Game::MainMenu>()->Init();
}
