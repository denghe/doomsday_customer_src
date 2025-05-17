#pragma once

namespace Game {

	inline void Test10::MakeUI() {
		ui->Clear();
		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos7 + XY{ 10, -10 }
			, gLooper.anchor7, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});

		//ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos8 + XY{ 0, -10 }
		//	, gLooper.anchor8, gLooper.btnCfg, U"Add 5000 HPBars", [this]() {
		//	});
	}

	inline void Test10::OnWindowSizeChanged() {
		MakeUI();
	}


	inline void Test10::Init() {
		StageInit({ 60, 60 });
		player.Emplace<Player_1>()->Init(this);

		ui.Emplace()->Init();
		MakeUI();

		// fill tile tex
		static constexpr int w{ 2 }, h{ 2 };
		std::array<xx::UV, w * h> data;
		data[0] = { 9, 0 };
		data[1] = { 5, 0 };
		data[2] = { 2, 0 };
		data[3] = { 7, 0 };
		tex.Emplace( xx::LoadGLTexture_Memory<GL_NEAREST, GL_REPEAT>(&data, w, h) );
	}

	void Test10::Update() {
		Stage::Update();
		offset += 0.1f;
	}


	inline void Test10::DrawCustomUI() {
		static constexpr XY tileSize{ 10, 13 };
		static constexpr XY gridSize{ 15, 8 };
		static constexpr XY mapSize{ tileSize * gridSize };
		static constexpr XY scale{ 10 };

		auto q = gLooper.ShaderBegin(gLooper.shaderTiles).Draw(gLooper.res_nums1, tex);
		q->pos = -mapSize * scale * 0.5f;
		q->scale = scale;
		q->tileSize = tileSize;
		q->mapSize = mapSize;
		q->tiling = 1;
		q->offset = offset;
	}

}
