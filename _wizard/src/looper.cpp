#include "pch.h"
#include "looper.h"

#include "game_shader_numbers.hpp"
#include "game_base_drawable.hpp"
#include "game_base_stageitem.hpp"
//#include "game_base_stage.hpp"
#include "game_main_menu.hpp"
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
	gLooper.title = "doomsday_customer_wizard";
	gLooper.Init();
	gLooper.Run<3>();
}
#endif

xx::Task<> Looper::MainTask() {

#ifdef __EMSCRIPTEN__
	res_nums1 = co_await AsyncLoadTextureFromUrl("res/nums1.png");
#else
	res_nums1 = LoadTexture("res/nums1.png");
#endif
	shaderNumbers.Init();

	co_await res.AsyncLoad("res/");

	rdd.Init(50, 128);

	btnCfg.frame = res.ui_button;
	btnCfg.center = { 2, 2, 2, 2 };

	scene.Emplace<Game::MainMenu>()->Init();
}
