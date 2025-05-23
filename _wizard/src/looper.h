﻿#pragma once
#include "pch.h"
#include "res_tp_frames.h"

struct Cfg : xx::GDesign<1920, 1080, 120> {
	static constexpr float unitSize{ 32.f };
	static constexpr float unitRadius{ unitSize / 2 };
};

#include "game_shader_numbers.h"
#include "game_space_index_box.h"
#include "game_space_index_circle.h"
#include "game_scene.h"
#include "game_stage.h"
#include "game_drawable.h"
#include "game_effect_numbers.h"
#include "game_main_menu.h"
// more include here ...
#include "_test1.h"

struct Looper : xx::Engine<Looper>, Cfg {
	xx::Task<> MainTask();

	xx::Ref<xx::GLTexture> res_texSpineBoy;
	spine::SkeletonData* res_skelSpineBoy{};

	xx::Ref<xx::GLTexture> res_texFrenchFries;
	spine::SkeletonData* res_skelFrenchFries{};

	xx::FrameBuffer fb;										// for light
    Game::Shader_QuadInstanceNumbers shaderNumbers;			// for effect damage number
	xx::Ref<xx::GLTexture> res_nums1;						// for effect damage number
	ResTpFrames res;										// res/pngs.blist + png

	xx::Scale9SpriteConfig btnCfg;
	xx::SpaceGridRingDiffuseData rdd;
	xx::Listi32<std::pair<float, Game::Drawable*>> yDraws;
};

extern Looper gLooper;
