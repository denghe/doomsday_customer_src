#pragma once

namespace Game {

	void Test7::Update() {
		Stage::Update();

		uvOffset += 0.1f;
	}




	inline void Test7::Init() {
		StageInit({ 60, 60 });
		player.Emplace<Player_1>()->Init(this);

		ui.Emplace()->Init();
		MakeUI();

		outlines.Emplace(std::make_pair(gLooper.res.player1, XY{ 200, -200 }));
		outlines.Emplace(std::make_pair(gLooper.res.bullet_coin5, XY{ 200, 0 }));
		outlines.Emplace(std::make_pair(gLooper.res.monster_hamburger, XY{ 200, 200 }));
	}




	inline void Test7::MakeUI() {
		ui->Clear();
		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos7 + XY{ 10, -10 }
			, gLooper.anchor7, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});

		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos8 + XY{ 0, -10 }
			, gLooper.anchor8, gLooper.btnCfg, U"Add More Outline", [this]() {
				AddMoreOutline();
			});
	}

	inline void Test7::OnWindowSizeChanged() {
		MakeUI();
	}



	inline void Test7::DrawCustomUI() {
		{
			auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstanceTilingOffset).Draw(gLooper.res.player1->tex);
			q->pos = {-200, 0};
			q->anchor = 0.5f;
			q->scale = 5;
			q->radians = 0;
			q->colorplus = 1.f;
			q->color = xx::RGBA8_White;
			q->texRect.data = gLooper.res.player1->textureRect.data;
			q->tiling = 25.f;
			q->offset = uvOffset;
		}
		
		for (auto len = outlines.len, i = 0; i < len; ++i) {
			auto& o = outlines[i];
			gLooper.ShaderBegin(gLooper.shaderQuadInstanceOutline).Draw(o.first, 1, xx::RGBA8_Red)->pos = o.second;
		}
	}

	void Test7::AddMoreOutline() {
		for (int32_t i = 0; i < 10000; ++i) {
			XY pos{ rnd.Next<float>(0, Cfg::width) - Cfg::width_2, rnd.Next<float>(0, Cfg::height) - Cfg::height_2 };
			auto idx = rnd.Next<int32_t>(0, gLooper.res._countof_font_outline_);
			outlines.Emplace(std::make_pair(gLooper.res.font_outline_[idx], pos));
		}
	}

}
