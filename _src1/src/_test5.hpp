#pragma once

namespace Game {

	inline void Test5::Init() {
		StageInit({ 60, 60 });
		player.Emplace<Player_1>()->Init(this);

		ui.Emplace()->Init();

		ui->MakeChildren<xx::Button>()->Init(10, Cfg::xy7m + XY{ 10, -10 }
			, Cfg::xy7a, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
		});

		// requires: after player init. need read player's STAT
		buffInfo.Emplace()->Init();
		buffShop.Emplace()->Init(player);
	}

	inline void Test5::DrawCustomUI() {
		if (buffShop) {
			gLooper.DrawNode(buffShop);
		}

		if (buffShop) {
			if (auto bt = buffShop->buffBag->content->TryGetMousePosItem()) {
				buffInfo->Set(*bt, gLooper.mouse.pos);
				gLooper.DrawNode(buffInfo);
			}
		}
	}
}
