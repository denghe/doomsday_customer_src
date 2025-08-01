﻿#include "pch.h"
#include "looper.h"

#include "shader_hpbar.hpp"
#include "shader_numbers.hpp"
#include "ui_stage.hpp"
#include "game_scene.hpp"
#include "game_effect_explosion.hpp"
#include "game_effect_numbers.hpp"
#include "game_drawable.hpp"
#include "game_block.hpp"
#include "game_map.hpp"
#include "game_player_bullet.hpp"
#include "game_player_weapon.hpp"
#include "game_player.hpp"
#include "game_monster_bullet.hpp"
#include "game_monster_bullet1_1.hpp"
#include "game_player_bullet_fire_a.hpp"
#include "game_player_bullet_fire_b.hpp"
#include "game_monster.hpp"
#include "game_monster1.hpp"
#include "game_stage.hpp"
#include "_game_main_menu.hpp"
#include "_game_choose_char.hpp"
// more include here ...
#include "_test1.hpp"
#include "_test2.hpp"

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
	res_nums1 = co_await AsyncLoadTextureFromUrl("res/nums1.png");
	res_hpbar = co_await AsyncLoadTextureFromUrl("res/hpbar.png");
	res_rocks = co_await AsyncLoadTextureFromUrl("res/rocks.png");
	res_bg_main = co_await AsyncLoadTextureFromUrl("res/bg_main.jpg");
	res_bg_fight_1 = co_await AsyncLoadTextureFromUrl("res/bg_fight_1.jpg");
	shaderNumbers.Init();
	shaderHPBar.Init();

	co_await res.AsyncLoad("res/");

	res_sound_button_1 = sound.Load(co_await AsyncDownloadFromUrl("res/button_1.ogg"));
	res_sound_shoot_1 = sound.Load(co_await AsyncDownloadFromUrl("res/shoot_1.ogg"));
	res_sound_shoot_2 = sound.Load(co_await AsyncDownloadFromUrl("res/shoot_2.ogg"));
	res_sound_hit_1 = sound.Load(co_await AsyncDownloadFromUrl("res/hit_1.ogg"));
	res_sound_bomb_1 = sound.Load(co_await AsyncDownloadFromUrl("res/bomb_1.ogg"));
	res_sound_bomb_2 = sound.Load(co_await AsyncDownloadFromUrl("res/bomb_2.ogg"));

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

	rdd.Init(50, Cfg::unitSizei);

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

	btnCfgSmall = btnCfg;
	btnCfgSmall.borderScale = { 2.f };
	btnCfgSmall.txtPadding = { 10, 3 };
	btnCfgSmall.txtPaddingRightBottom = { 10, 5 };
	btnCfgSmall.txtScale = { 1 };
	btnCfgSmall.iconPadding = { 3 };



	cfg_btnNormal.Emplace();
	cfg_btnNormal->frame = res.ui_button_n;
	cfg_btnNormal->texScale = { 1, 1 };
	cfg_btnNormal->center = { 2, 2, 2, 2 };
	cfg_btnNormal->color = { 255, 255, 255, 255 };
	cfg_btnNormal->borderScale = { 4.f };
	cfg_btnNormal->txtColor = { xx::RGBA8_White };
	cfg_btnNormal->txtPadding = { 20, 5 };
	cfg_btnNormal->txtPaddingRightBottom = { 20, 10 };
	cfg_btnNormal->txtScale = { 2 };
	cfg_btnNormal->iconPadding = { 5 };

	cfg_btnHighlight.Emplace();
	*cfg_btnHighlight = *cfg_btnNormal;
	cfg_btnHighlight->frame = res.ui_button_h;

	cfg_imgBtnNormal.Emplace();
	*cfg_imgBtnNormal = *cfg_btnNormal;
	cfg_imgBtnNormal->frame = res.ui_imgbtn_n;

	cfg_imgBtnHighlight.Emplace();
	*cfg_imgBtnHighlight = *cfg_btnNormal;
	cfg_imgBtnHighlight->frame = res.ui_imgbtn_h;

	cfg_sliderBar.Emplace();
	*cfg_sliderBar = *cfg_btnNormal;
	cfg_sliderBar->frame = res.ui_slider_bar;

	cfg_sliderBlock.Emplace();
	*cfg_sliderBlock = *cfg_btnNormal;
	cfg_sliderBlock->frame = res.ui_slider_block;


	scene.Emplace<Game::MainMenu>()->Init();
}
