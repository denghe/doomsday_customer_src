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
		DrawDevelop();

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
		root->MakeChildren<xx::Button>()->Init(1, p, a, c, U"*2", [&]() {
			stage->frameDelay = Cfg::frameDelay * 2.f;
			});
		p.y -= 50;
		root->MakeChildren<xx::Button>()->Init(1, p, a, c, U"*1", [&]() {
			stage->frameDelay = Cfg::frameDelay;
			});
		p.y -= 50;
		root->MakeChildren<xx::Button>()->Init(1, p, a, c, U"*0.5", [&]() {
			stage->frameDelay = Cfg::frameDelay * 0.5f;
			});
		p.y -= 50;
		root->MakeChildren<xx::Button>()->Init(1, p, a, c, U"*0.2", [&]() {
			stage->frameDelay = Cfg::frameDelay * 0.2f;
			});
		p.y -= 50;
		root->MakeChildren<xx::Button>()->Init(1, p, a, c, U"Switch Light", [&]() {
			stage->disableLight = !stage->disableLight;
			});

		p = stage->pos7 + XY{ 10, -100 };
		a = stage->anchor7;
		{
			auto& b1 = root->MakeChildren<xx::Button>();
			b1->Init(1, p, a, c, U" + ", [&]() {
				++stage->player->weapon->pwp.projectileAmount;
			});
			auto& b2 = root->MakeChildren<xx::Button>();
			b2->Init(1, p + XY{ b1->size.x, 0 }, a, c, U" - ", [&]() {
				if (stage->player->weapon->pwp.projectileAmount > 1.f) {
					--stage->player->weapon->pwp.projectileAmount;
				}
			});
			labelProjectileAmount = root->MakeChildren<xx::Label>();
			labelProjectileAmount->Init(1, p + XY{ b1->size.x + b2->size.x + 2, 0 }, 1.f, a, xx::RGBA8_Green, U"");
		}
		p.y -= 50;
		{
			auto& b1 = root->MakeChildren<xx::Button>();
			b1->Init(1, p, a, c, U" + ", [&]() {
				++stage->player->weapon->pwp.shootSpeed;
			});
			auto& b2 = root->MakeChildren<xx::Button>();
			b2->Init(1, p + XY{ b1->size.x, 0 }, a, c, U" - ", [&]() {
				if (stage->player->weapon->pwp.shootSpeed > 1.f) {
					--stage->player->weapon->pwp.shootSpeed;
				}
			});
			labelShootSpeed = root->MakeChildren<xx::Label>();
			labelShootSpeed->Init(1, p + XY{ b1->size.x + b2->size.x + 2, 0 }, 1.f, a, xx::RGBA8_Green, U"");
		}
		p.y -= 50;
		{
			auto& b1 = root->MakeChildren<xx::Button>();
			b1->Init(1, p, a, c, U" + ", [&]() {
				++stage->player->weapon->pwp.manaCost;
			});
			auto& b2 = root->MakeChildren<xx::Button>();
			b2->Init(1, p + XY{ b1->size.x, 0 }, a, c, U" - ", [&]() {
				if (stage->player->weapon->pwp.manaCost > 0.f) {
					--stage->player->weapon->pwp.manaCost;
				}
			});
			labelManaCost = root->MakeChildren<xx::Label>();
			labelManaCost->Init(1, p + XY{ b1->size.x + b2->size.x + 2, 0 }, 1.f, a, xx::RGBA8_Green, U"");
		}
		p.y -= 50;
		{
			auto& b1 = root->MakeChildren<xx::Button>();
			b1->Init(1, p, a, c, U" + ", [&]() {
				++stage->player->weapon->pwp.spread;
			});
			auto& b2 = root->MakeChildren<xx::Button>();
			b2->Init(1, p + XY{ b1->size.x, 0 }, a, c, U" - ", [&]() {
				if (stage->player->weapon->pwp.spread > 0.f) {
					--stage->player->weapon->pwp.spread;
				}
			});
			labelSpread = root->MakeChildren<xx::Label>();
			labelSpread->Init(1, p + XY{ b1->size.x + b2->size.x + 2, 0 }, 1.f, a, xx::RGBA8_Green, U"");
		}
	}

	inline void Stage::DrawDevelop() {
		labelProjectileAmount->SetText(
			xx::ToString("projectileAmount = ", stage->player->weapon->pwp.projectileAmount)
		);
		labelShootSpeed->SetText(
			xx::ToString("shootSpeed = ", stage->player->weapon->pwp.shootSpeed)
		);
		labelManaCost->SetText(
			xx::ToString("manaCost = ", stage->player->weapon->pwp.manaCost)
		);
		labelSpread->SetText(
			xx::ToString("spread = ", stage->player->weapon->pwp.spread)
		);
	}
}
