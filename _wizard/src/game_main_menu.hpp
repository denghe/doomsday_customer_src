#pragma once

namespace Game {

	inline void MainMenu::MakeUI() {
		ui.Emplace()->Init(0, {}, scale);

		ui->MakeChildren<xx::Button>()->Init(2, pos5 + XY{ 0, 100 }, anchor5, gLooper.btnCfg, U"new game").onClicked = [&]() {
			gLooper.DelaySwitchTo<Stage>(); 
			};
	}

	inline void MainMenu::OnWindowSizeChanged() {
		MakeUI();
	}

	inline void MainMenu::Init() {
		UpdateScale();
		MakeUI();
	}

	void MainMenu::Update() {
		offset -= Cfg::frameDelay * 800;
	}

	inline void MainMenu::Draw() {
		gLooper.DrawNode(ui);
		{
			XY pos{ -100, 0 };
			auto d = gLooper.mouse.pos - pos;
			auto mag2 = d.x * d.x + d.y * d.y;
			auto dist = std::sqrtf(d.x * d.x + d.y * d.y);
			auto radians = std::atan2f(d.y, d.x);
			auto tiling = dist / ResTpFrames::_size_laser1.x;
			auto o = tiling > 0 ? offset / tiling : 0;
			auto f = gLooper.res.laser1;
			auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstanceTilingOffset).Draw(f->tex, 1);
			q->pos = pos;
			q->anchor = { 0, 0.5f };
			q->scale = { tiling, 1 };
			q->radians = -radians;
			q->colorplus = 1.f;
			q->color = {115,223,255,255};
			q->texRect.data = f->textureRect.data;
			q->tiling = { tiling, 1 };
			q->offset = { o, 0 };
		}
		{
			XY pos{ 100, 0 };
			auto d = gLooper.mouse.pos - pos;
			auto mag2 = d.x * d.x + d.y * d.y;
			auto dist = std::sqrtf(d.x * d.x + d.y * d.y);
			auto radians = std::atan2f(d.y, d.x);
			auto tiling = dist / ResTpFrames::_size_laser1.x;
			auto o = tiling > 0 ? offset / tiling : 0;
			auto f = gLooper.res.laser2;
			auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstanceTilingOffset).Draw(f->tex, 1);
			q->pos = pos;
			q->anchor = { 0, 0.5f };
			q->scale = { tiling, 1 };
			q->radians = -radians;
			q->colorplus = 1.f;
			q->color = xx::RGBA8_Red;
			q->texRect.data = f->textureRect.data;
			q->tiling = { tiling, 1 };
			q->offset = { o, 0 };
		}
	}

}
