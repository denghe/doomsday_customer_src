#include "pch.h"
#include "looper.h"

#include "game_shader_numbers.hpp"
#include "game_effect_numbers.hpp"
#include "game_scene.hpp"
#include "game_stage.hpp"
#include "game_drawable.hpp"
#include "game_main_menu.hpp"
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


	// load spine res & parse
	auto& se = xx::gSpineEnv;
	se.Init();
	co_await se.AsyncLoad("res/spineboy-ess", res_skelSpineBoy, res_texSpineBoy);
	co_await se.AsyncLoad("res/french_fries", res_skelFrenchFries, res_texFrenchFries);

	rdd.Init(50, 128);

	fb.Init();

	btnCfg.frame = res.ui_button;
	btnCfg.texScale = { 1, 1 };
	btnCfg.center = { 2, 2, 2, 2 };
	btnCfg.color = { 127, 127, 127, 255 };
	btnCfg.borderScale = { 4.f };
	btnCfg.txtColor = { xx::RGBA8_White };
	btnCfg.txtPadding = { 20, 5 };
	btnCfg.txtPaddingRightBottom = { 20, 10 };
	btnCfg.txtScale = { 2 };
	btnCfg.iconPadding = { 5 };

	scene.Emplace<Game::MainMenu>()->Init();
}
