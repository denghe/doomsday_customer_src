#pragma once

namespace Game {


	inline void Test6::MakeUI() {
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

	inline void Test6::OnWindowSizeChanged() {
		MakeUI();
	}

	inline void Test6::Init() {
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

	inline void Test6::Draw() {
		Stage::Draw();

		for (int32_t i = 0; i < 500000; ++i) {
			XY basePos{ rnd.Next<float>(-800, 800), rnd.Next<float>(-400, 400) };
			auto texId = gLooper.res._texid_player1;
			static constexpr auto uvr = gLooper.res._uvrect_player1;

			auto [bi, vs, is] = gLooper.ShaderBegin(gLooper.shaderVertexs).Draw(texId, 4, 6);
			vs[0] = { .pos = basePos + XY{-10, +10}, .uv = {uvr.x, uvr.y}, .color = xx::RGBA8_White };
			vs[1] = { .pos = basePos + XY{+10, +10}, .uv = {uvr.x + uvr.w, uvr.y}, .color = xx::RGBA8_White };
			vs[2] = { .pos = basePos + XY{+10, -10}, .uv = {uvr.x + uvr.w, uvr.y + uvr.h}, .color = xx::RGBA8_White };
			vs[3] = { .pos = basePos + XY{-10, -10}, .uv = {uvr.x, uvr.y + uvr.h}, .color = xx::RGBA8_White };
			is[0] = bi + 0;
			is[1] = bi + 1;
			is[2] = bi + 3;
			is[3] = bi + 1;
			is[4] = bi + 2;
			is[5] = bi + 3;
		}
	}
}
