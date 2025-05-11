#pragma once

namespace Game {

	void Test7::Update() {
		Stage::Update();

		offsetX++;
	}




	inline void Test7::Init() {
		fb.Init();
		ui.Emplace()->Init();
		MakeUI();

		gridSize = { 60, 60 };
		mapSize = 128 * gridSize;

		ground.Emplace()->Init(this, mapSize, gLooper.res.ground_cell3);
		player.Emplace<Player_1>()->Init(this);

		EnvGrass::GenGrass(this, 5);

		camera.scale = Cfg::defaultScale;
		camera.mapSize = mapSize;
		camera.newOriginal = camera.original = mapSize * 0.5f;


	}




	inline void Test7::MakeUI() {
		ui->Clear();
		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos7 + XY{ 10, -10 }
			, gLooper.anchor7, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});

		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos8 + XY{ 0, -10 }
			, gLooper.anchor8, gLooper.btnCfg, U"Add More Grass", [this]() {
				EnvGrass::GenGrass(this, 5);
			});
	}

	inline void Test7::OnWindowSizeChanged() {
		MakeUI();
	}



	inline void Test7::DrawCustomUI() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstanceTilingOffset).Draw(gLooper.res.player1->tex, 1);
		q->pos = {};
		q->anchor = 0.5f;
		q->scale = 5;
		q->radians = 0;
		q->colorplus = 1.f;
		q->color = xx::RGBA8_White;
		q->texRect.data = gLooper.res.player1->textureRect.data;
		q->tiling = 5.f;
		q->offset = offsetX;
	}
}
