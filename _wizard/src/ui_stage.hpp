#pragma once

namespace UI {

	inline void HPBar::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderHPBar).Draw(v1, v2);
		q->pos = worldMinXY;
		q->scale = worldScale;
		q->color = color;
	}

	inline void Stage::Init(xx::Weak<Game::Stage> stage_) {
		stage = std::move(stage_);
		MakeUI();
	}

	inline void Stage::OnWindowSizeChanged() {
		MakeUI();
	}

	inline void Stage::Draw() {
		// todo: draw hp, mp

		auto& pp = stage->player->pp;
		hpBar->v1 = (uint32_t)pp.health;
		hpBar->v2 = (uint32_t)pp.healthMax;
		mpBar->v1 = (uint32_t)pp.mana;
		mpBar->v2 = (uint32_t)pp.manaMax;

		gameTips->SetText(xx::ToString("control: SPACE + ASDW + mouse. "
			, "m = ", stage->monsters.items.len
			, " mb = ", stage->monsterBullets.items.len
			, " pb = ", stage->playerBullets.len
			, " e = ", stage->effectExplosions.len
		));

		gLooper.DrawNode(root);
	}

	XX_INLINE void Stage::SetRoundInfo(std::u32string_view const& txt_) {
		roundInfo->SetText(txt_);
	}
	XX_INLINE void Stage::SetRoundInfo(std::string_view const& txt_) {
		roundInfo->SetText(txt_);
	}
	XX_INLINE void Stage::SetRoundInfo() {
		roundInfo->SetText();
	}

	inline void Stage::MakeDevelopUI() {
		auto a = stage->anchor9;
		auto c = gLooper.btnCfgSmall;
		auto p = stage->pos9 + XY{ -10, -10 };
		gameTips = root->MakeChildren<xx::Label>();
		gameTips->Init(1, p, 1.f, a, xx::RGBA8_Green, U"");
		p.y -= 50;
		root->MakeChildren<xx::Button>()->Init(1, p, a, c, U"exit", [&]() {
			gLooper.DelaySwitchTo<Game::MainMenu>();
			});
		p.y -= 50;
		root->MakeChildren<xx::Button>()->Init(1, p, a, c, U"GameSpeed*100", [&]() {
			stage->frameDelay = Cfg::frameDelay * 100.f;
			});
		p.y -= 50;
		root->MakeChildren<xx::Button>()->Init(1, p, a, c, U"*10", [&]() {
			stage->frameDelay = Cfg::frameDelay * 10.f;
			});
		p.y -= 50;
		root->MakeChildren<xx::Button>()->Init(1, p, a, c, U"*5", [&]() {
			stage->frameDelay = Cfg::frameDelay * 5.f;
			});
		p.y -= 50;
		root->MakeChildren<xx::Button>()->Init(1, p, a, c, U"*1", [&]() {
			stage->frameDelay = Cfg::frameDelay;
			});
		p.y -= 50;
		root->MakeChildren<xx::Button>()->Init(1, p, a, c, U"*0.7", [&]() {
			stage->frameDelay = Cfg::frameDelay * 0.7f;
			});
		p.y -= 50;
		root->MakeChildren<xx::Button>()->Init(1, p, a, c, U"*0.3", [&]() {
			stage->frameDelay = Cfg::frameDelay * 0.3f;
			});
		p.y -= 50;
		root->MakeChildren<xx::Button>()->Init(1, p, a, c, U"Switch Light", [&]() {
			stage->disableLight = !stage->disableLight;
			});
	}

	inline void Stage::MakeUI() {
		root.Emplace()->Init(0, {}, stage->scale);
		MakeDevelopUI();

		if (roundInfo) {
			root->AddChildren(roundInfo);
			roundInfo->position = stage->pos5 + XY{ 0, 450 };
			roundInfo->FillTransRecursive();
		}
		else {
			roundInfo = root->MakeChildren<xx::Label>();
			roundInfo->Init(1, stage->pos5 + XY{ 0, 450 }, 2.f, stage->anchor5, xx::RGBA8_White, "");
		}

		static constexpr auto barScale = 2;
		hpBar = root->MakeChildren<HPBar>();
		hpBar->Init(1, stage->pos7 + XY{ 10, -10 }, barScale, {}, { 110, 15 });
		hpBar->color = xx::RGBA8_Red;

		mpBar = root->MakeChildren<HPBar>();
		mpBar->Init(1, stage->pos7 + XY{ 10, -20 - 15 * barScale }, barScale, {}, { 110, 15 });
		mpBar->color = xx::RGBA8_Blue;
	}

}
