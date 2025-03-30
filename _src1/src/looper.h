#pragma once
#include "pch.h"
#include "res_tp_frames.h"

// design size: 4k
// 1k for windows mode & easy test
struct Cfg : xx::GDesign<1920, 1080, 120> {
	static constexpr float unitSize{ 128.f };
	static constexpr float globalScale{ 1.f };
};

#include "game_main_menu.h"
#include "game_ground.h"
#include "game_player.h"
#include "game_stage1.h"
// more include here ...

struct Looper : xx::Engine<Looper>, Cfg {
	xx::Task<> MainTask();

	ResTpFrames res;
	xx::Scale9SpriteConfig btnCfg;
};

extern Looper gLooper;
