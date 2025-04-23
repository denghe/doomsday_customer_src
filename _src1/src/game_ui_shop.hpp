#pragma once

namespace Game {
	void UI_ShopPanel::Init(Stage* stage_) {
		stage = stage_;
	}

	void UI_ShopPanel::Popup() {
		stage->paused = true;								// pause

		ui.Emplace()->Init();
		ui->MakeChildren<xx::SwallowButton>()->Init(1).onClicked = [] {};		// avoid user click other buttons

		static constexpr float leftPos{ -630 };
		static constexpr float rightPos{ 630 };
		static constexpr float topPos{ 340 };
		static constexpr float bottomPos{ -340 };

		static constexpr auto bgScale = 1080.f * 0.95f / gLooper.res._size_ui_paper.x;
		ui->MakeChildren<xx::Image>()->Init(2, {}, bgScale, 0.5f, gLooper.res.ui_paper, xx::ImageRadians::PiDiv2);

		static constexpr XY goodsBtnSize{ 300, 450 };
		static constexpr float goodsNameWidth{ 150 };
		static constexpr float goodsBtnMargin{ 20 };
		static constexpr XY goodsBtnOffset{ leftPos + (goodsBtnSize.x * 3 + goodsBtnMargin * 2) / 2, 0 };
		static constexpr XY goodsBtnPoss[3] = { { goodsBtnOffset + XY{ -goodsBtnSize.x * 1.f - goodsBtnMargin, 0 } }, { goodsBtnOffset + XY{ 0, 0 } }, { goodsBtnOffset + XY{ goodsBtnSize.x * 1.f + goodsBtnMargin, 0 } } };
		static constexpr XY goodsBtnContentPoss[3] = { goodsBtnPoss[0] + XY{ 0, 200 }, goodsBtnPoss[1] + XY{ 0, 200 }, goodsBtnPoss[2] + XY{ 0, 200 } };

		ui->MakeChildren<xx::Label>()->Init(3, { goodsBtnOffset.x, topPos }, 4.f, 0.5f, xx::RGBA8_Black, U"商店");

		ui->MakeChildren<xx::IconButton>()->Init(3, { rightPos, topPos }, { 1, 0.5f }, gLooper.btnCfg3i, gLooper.res.ui_refresh, U"$123", {}, xx::RGBA8_Black).onClicked = [&]() {
			// todo
		};


		ui->MakeChildren<xx::EmptyButton>()->Init(3, goodsBtnPoss[0], 0.5f, gLooper.btnCfg4, goodsBtnSize);
		ui->MakeChildren<xx::EmptyButton>()->Init(3, goodsBtnPoss[1], 0.5f, gLooper.btnCfg4, goodsBtnSize);
		ui->MakeChildren<xx::EmptyButton>()->Init(3, goodsBtnPoss[2], 0.5f, gLooper.btnCfg4, goodsBtnSize);

		static constexpr float goodsBtnContentWidth{ goodsBtnSize.x - goodsBtnMargin * 2 };
		auto goodsBtnIconFrame = gLooper.res.ui_money;
		static constexpr float goodsBtnIconDrawHeight{ 100 };
		auto goodsBtnIconScale = goodsBtnIconDrawHeight / goodsBtnIconFrame->spriteSize.y;
		auto goodsBtnIconDrawWidth = goodsBtnIconFrame->spriteSize.x * goodsBtnIconScale;
		{
			// goods1
			auto& rich = ui->MakeChildren<xx::RichLabel>()->Init(4, goodsBtnContentPoss[0], 1, { 0.5f, 1 }, goodsBtnContentWidth);
			rich.SetOffset(goodsBtnContentWidth / 2 - goodsBtnIconDrawWidth / 2).AddPicture(gLooper.res.ui_money, goodsBtnIconScale)
				.AddLimitedWidthText(U"\n攻击倍率", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"+20%", 2, xx::RGBA8_Red)
				.AddLimitedWidthText(U"\n每秒回血", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"-5%", 2, xx::RGBA8_Green)
				.AddLimitedWidthText(U"\n最大血量", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"-5%", 2, xx::RGBA8_Green)
				.Commit();
		}
		{
			// goods2
			auto& rich = ui->MakeChildren<xx::RichLabel>()->Init(4, goodsBtnContentPoss[1], 1, { 0.5f, 1 }, goodsBtnContentWidth);
			rich.SetOffset(goodsBtnContentWidth / 2 - goodsBtnIconDrawWidth / 2).AddPicture(gLooper.res.ui_money, goodsBtnIconScale)
				.AddLimitedWidthText(U"\n攻击倍率", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"+20%", 2, xx::RGBA8_Red)
				.AddLimitedWidthText(U"\n每秒回血", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"-5%", 2, xx::RGBA8_Green)
				.AddLimitedWidthText(U"\n最大血量", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"-5%", 2, xx::RGBA8_Green)
				.Commit();
		}
		{
			// goods3
			auto& rich = ui->MakeChildren<xx::RichLabel>()->Init(4, goodsBtnContentPoss[2], 1, { 0.5f, 1 }, goodsBtnContentWidth);
			rich.SetOffset(goodsBtnContentWidth / 2 - goodsBtnIconDrawWidth / 2).AddPicture(gLooper.res.ui_money, goodsBtnIconScale)
				.AddLimitedWidthText(U"\n攻击倍率", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"+20%", 2, xx::RGBA8_Red)
				.AddLimitedWidthText(U"\n每秒回血", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"-5%", 2, xx::RGBA8_Green)
				.AddLimitedWidthText(U"\n最大血量", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"-5%", 2, xx::RGBA8_Green)
				.Commit();
		}

		{
			// player stat
			static constexpr XY statRichPos{ rightPos, goodsBtnSize.y / 2 };
			auto& rich = ui->MakeChildren<xx::RichLabel>()->Init(4, statRichPos, 1, { 1, 1 }, goodsBtnContentWidth);
			rich.AddLimitedWidthText(U"\n攻击倍率", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"+20%", 2, xx::RGBA8_Red)
				.AddLimitedWidthText(U"\n每秒回血", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"-5%", 2, xx::RGBA8_Green)
				.AddLimitedWidthText(U"\n最大血量", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"-5%", 2, xx::RGBA8_Green)
				.AddLimitedWidthText(U"\n幸运点", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"5", 2, xx::RGBA8_Green)
				.AddLimitedWidthText(U"\n暴击率", goodsNameWidth, 2, xx::RGBA8_Black).AddRightText(U"10%", 2, xx::RGBA8_Green)
				.Commit();
		}

		auto& btnContinue = ui->MakeChildren<xx::Button>()->Init(3, { rightPos, bottomPos }, { 1, 0.5f }, gLooper.btnCfg3, U"继续");
		btnContinue.onClicked = [this] {
			stage->paused = false;							// resume
			ui.Reset();										// close popup panel
		};

		ui->MakeChildren<xx::Button>()->Init(3, { rightPos - 30 - btnContinue.size.x, bottomPos }, { 1, 0.5f }, gLooper.btnCfg3, U"Rent").onClicked = [this] {
			// todo
		};

		static constexpr float moneyIconWidth{ 50 };
		static constexpr float moneyIconScale{ 50 / gLooper.res._size_ui_money.x };
		static constexpr float moneyIconTextMargin{ 30 };
		static constexpr XY moneyIconPos{ leftPos + moneyIconWidth, bottomPos };
		static constexpr XY moneyTextPos{ moneyIconPos.x + moneyIconTextMargin, bottomPos };
		ui->MakeChildren<xx::Image>()->Init(2, moneyIconPos, moneyIconScale, { 1, 0.5f }, gLooper.res.ui_money);
		ui->MakeChildren<xx::Label>()->Init(3, moneyTextPos, 2.f, { 0, 0.5f }, xx::RGBA8_Black, U"123456");
	}

	void UI_ShopPanel::TryDraw() {
		if (ui) {
			gLooper.DrawNode(ui);
		}
	}

}
