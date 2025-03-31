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
#include "game_stage1.hpp"

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
	gLooper.Run<false>();
}
#endif

xx::Task<> Looper::MainTask() {
	co_await res.AsyncLoad("res/");

	btnCfg.frame = res.button;
	btnCfg.texScale = { 0.99, 0.99 };
	btnCfg.center = { 2, 3, 2, 2 };
	btnCfg.color = { 0x5f, 0x15, 0xd9, 0xff };

	clearColor = { 33, 33, 33, 255 };

	scene.Emplace<Game::MainMenu>()->Init();
}
