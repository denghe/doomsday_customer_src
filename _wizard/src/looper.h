#pragma once
#include "pch.h"
#include "res_tp_frames.h"

struct Cfg : xx::GDesign<1920, 1080, 120> {
	static constexpr float unitSize{ 32.f };
	static constexpr float unitRadius{ unitSize / 2 };
};

#include "game_shader_numbers.h"
#include "game_space_box.h"
#include "game_space_circle.h"
#include "game_effect_text.h"
#include "game_main_menu.h"
#include "game_base.h"
// more include here ...

struct Looper : xx::Engine<Looper>, Cfg {
	xx::Task<> MainTask();

	xx::FrameBuffer fb;

    Game::Shader_QuadInstanceNumbers shaderNumbers;

	// res/?????
	xx::Ref<xx::GLTexture> res_nums1;

	// res/pngs.blist + png
	ResTpFrames res;

	xx::Scale9SpriteConfig btnCfg;
	xx::SpaceGridRingDiffuseData rdd;
	xx::Listi32<std::pair<float, Game::Drawable*>> yDraws;

	float globalScale{};
};

extern Looper gLooper;
