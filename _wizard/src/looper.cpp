#include "pch.h"
#include "looper.h"

#include "game_shader_numbers.hpp"
#include "game_scene.hpp"
#include "game_effect_explosion.hpp"
#include "game_drawable.hpp"
#include "game_block.hpp"
#include "game_map.hpp"
#include "game_player_bullet.hpp"
#include "game_player_weapon.hpp"
#include "game_player.hpp"
#include "game_monster_formation.hpp"
#include "game_monster_bullet.hpp"
#include "game_monster.hpp"
//#include "game_effect_numbers.hpp"
#include "game_stage.hpp"
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

	res_sound_laster_gun_1 = sound.Load(co_await AsyncDownloadFromUrl("res/laster_gun_1.ogg"));
	res_sound_monster_die_1 = sound.Load(co_await AsyncDownloadFromUrl("res/monster_die_1.ogg"));

	// if include spine.h, always need Init()
	auto& se = xx::gSpineEnv;
	se.Init();
	// enable for test1
#if 0
	// load spine res & parse
	co_await se.AsyncLoad("res/spineboy-ess", res_SpineBoy_skel, res_SpineBoy_tex);
	res_SpineBoy_walk = res_SpineBoy_skel->findAnimation("walk");
	res_SpineBoy_jump = res_SpineBoy_skel->findAnimation("jump");
	res_SpineBoy_run = res_SpineBoy_skel->findAnimation("run");

	co_await se.AsyncLoad("res/french_fries", res_FrenchFries_skel, res_FrenchFries_tex);
	res_FrenchFries_idle = res_FrenchFries_skel->findAnimation("idle");
	res_FrenchFries_walk = res_FrenchFries_skel->findAnimation("walk");

	co_await se.AsyncLoad<true>("res/slime1", res_Slime1_skel, res_Slime1_tex);
	res_Slime1_idle = res_Slime1_skel->findAnimation("idle");

	co_await se.AsyncLoad<true>("res/dragon-ess", res_Dragon_skel, res_Dragon_tex);
	res_Dragon_flying = res_Dragon_skel->findAnimation("flying");
#endif

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
