#pragma once
#include "pch.h"
#include "res_tp_frames.h"

struct Cfg : xx::GDesign<1920, 1080, 120> {
	static constexpr float unitSize{ 64.f };
	static constexpr int32_t unitSizei{ int32_t(unitSize) };
	static constexpr float unitRadius{ unitSize / 2 };
};

#include "shader_hpbar.h"
#include "shader_numbers.h"
#include "ui_lang.h"
#include "ui_stage.h"
#include "game_space_index_box.h"
#include "game_space_index_circle.h"
#include "game_scene.h"
#include "game_shaker.h"
#include "game_camera.h"
#include "game_effect_explosion.h"
#include "game_effect_numbers.h"
#include "game_drawable.h"
#include "game_block.h"
#include "game_map.h"
#include "game_monster_properties.h"
#include "game_player_weapon_properties.h"
#include "game_player_bullet.h"
#include "game_player_bullet_fire_a.h"
#include "game_player_bullet_fire_b.h"
#include "game_player_weapon.h"
#include "game_player_properties.h"
#include "game_player.h"
#include "game_monster_bullet.h"
#include "game_monster_bullet1_1.h"
#include "game_monster.h"
#include "game_monster1.h"
#include "game_stage.h"
#include "_game_main_menu.h"
#include "_game_choose_char.h"
// more include here ...
#include "_test1.h"
#include "_test2.h"
#include "_test3.h"

struct Looper : xx::Engine<Looper>, Cfg {
	xx::Task<> MainTask();

	xx::Ref<xx::GLTexture> res_SpineBoy_tex;
	spine::SkeletonData* res_SpineBoy_skel{};
	spine::Animation* res_SpineBoy_walk{};
	spine::Animation* res_SpineBoy_jump{};
	spine::Animation* res_SpineBoy_run{};

	xx::Ref<xx::GLTexture> res_FrenchFries_tex;
	spine::SkeletonData* res_FrenchFries_skel{};
	spine::Animation* res_FrenchFries_idle{};
	spine::Animation* res_FrenchFries_walk{};

	xx::Ref<xx::GLTexture> res_Slime1_tex;
	spine::SkeletonData* res_Slime1_skel{};
	spine::Animation* res_Slime1_idle{};

	xx::Ref<xx::GLTexture> res_Dragon_tex;
	spine::SkeletonData* res_Dragon_skel{};
	spine::Animation* res_Dragon_flying{};

	xx::Sound sound;
	xx::Shared<xx::SoundSource> res_sound_button_1;
	xx::Shared<xx::SoundSource> res_sound_shoot_1;
	xx::Shared<xx::SoundSource> res_sound_shoot_2;
	xx::Shared<xx::SoundSource> res_sound_hit_1;
	xx::Shared<xx::SoundSource> res_sound_bomb_1;
	xx::Shared<xx::SoundSource> res_sound_bomb_2;


	xx::FrameBuffer fb;										// for light
    Game::Shader_QuadInstanceNumbers shaderNumbers;			// for effect damage number
	xx::Ref<xx::GLTexture> res_nums1;						// for effect damage number
    Game::Shader_QuadInstanceHPBar shaderHPBar;				// for hp & mp bar
	xx::Ref<xx::GLTexture> res_hpbar;						// for hp bar
	xx::Ref<xx::GLTexture> res_rocks;						// for bg blocks
	xx::Ref<xx::GLTexture> res_bg_main;						// for main menu
	xx::Ref<xx::GLTexture> res_bg_fight_1;					// for fight
	ResTpFrames res;										// res/pngs.blist + png

	xx::Scale9SpriteConfig btnCfg, btnCfgSmall;
	xx::Ref<xx::Scale9SpriteConfig> cfg_btnNormal, cfg_btnHighlight;
	xx::Ref<xx::Scale9SpriteConfig> cfg_imgBtnNormal, cfg_imgBtnHighlight;
	xx::Ref<xx::Scale9SpriteConfig> cfg_sliderBar, cfg_sliderBlock;
	xx::SpaceGridRingDiffuseData rdd;
	xx::Listi32<std::pair<float, Game::Drawable*>> yDraws;


	// todo: global context?
	int32_t charSelectedIndex{};
};

extern Looper gLooper;
