#pragma once

namespace Game {

	inline void Test10::MakeUI() {
		ui->Clear();
		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos7 + XY{ 10, -10 }
			, gLooper.anchor7, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});

		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos8 + XY{ -250, -10 }
			, gLooper.anchor8, gLooper.btnCfg, U"set ground 1", [this]() {
				ground.Emplace<Ground1>()->Init(this, gLooper.res.ground_cell3);
			});

		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos8 + XY{ 0, -10 }
			, gLooper.anchor8, gLooper.btnCfg, U"set ground 2", [this]() {
				ground.Emplace<Ground2>()->Init(this);
			});

		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos8 + XY{ 250, -10 }
			, gLooper.anchor8, gLooper.btnCfg, U"clear all grass", [this]() {
				ground->Clear();
			});

		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos8 + XY{ -200, -70 }
			, gLooper.anchor8, gLooper.btnCfg, U"add some grass1", [this]() {
				ground->AddSomeGrass(0.1, 1);
			});

		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos8 + XY{ 200, -70 }
			, gLooper.anchor8, gLooper.btnCfg, U"add some grass2", [this]() {
				ground->AddSomeGrass(1, 2);
			});
	}

	inline void Test10::OnWindowSizeChanged() {
		MakeUI();
	}


	// scale 9 map:
	// 00 10 20
	// 01 11 21
	// 02 12 22

	//// map logic size
	//static constexpr xx::XYi miniMapSize{ 8, 5 };

	//// tex aligned size
	//static constexpr xx::XYi miniTexSize{(miniMapSize.x + 3) & ~3u, miniMapSize.y };

	inline void Test10::Init() {

		//// malloc tex mem
		//auto d = std::make_unique<xx::UV[]>(miniTexSize.x * miniTexSize.y);

		//// fill tex mem
		//for (int y = 0; y < miniMapSize.y; ++y) {
		//	uint16_t v{ 1 };
		//	if (y == 0) v = 0; else if (y == miniMapSize.y - 1) v = 2;
		//	for (int x = 0; x < miniMapSize.x; ++x) {
		//		uint16_t u{ 1 };
		//		if (x == 0) u = 0; else if (x == miniMapSize.x - 1) u = 2;
		//		d[y * miniTexSize.x + x] = { u, v };
		//	}
		//}

		//// copy & create tex
		//tex.Emplace(xx::LoadGLTexture_Memory<GL_NEAREST, GL_REPEAT>(d.get(), miniTexSize.x, miniTexSize.y));



		StageInit<false>({ 15, 15 });
		ground.Emplace<Ground2>()->Init(this);
		player.Emplace<Player_1>()->Init(this);
		player->movementSpeedPerFrame = 1000.f / Cfg::fps;

		ui.Emplace()->Init();
		MakeUI();
	}

	void Test10::Update() {
		Stage::Update();
		//offset += 0.1f;
	}


	inline void Test10::DrawCustomUI() {
		//static constexpr XY tileSize{ 128, 128 };
		//static constexpr XY mapSize{ tileSize * miniMapSize };
		//static constexpr XY scale{ 1 };

		//auto q = gLooper.ShaderBegin(gLooper.shaderTiles).Draw(gLooper.res_map_01, tex);
		//q->pos = -mapSize * scale * 0.5f;
		//q->scale = scale;
		//q->tileSize = tileSize;
		//q->mapSize = mapSize;
		//q->tiling = 1;
		//q->offset = offset;
	}

}
