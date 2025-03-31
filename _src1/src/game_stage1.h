#pragma once

namespace Game {

	struct Stage1 : StageBase {
		static constexpr XYi cCellSize{ 128, 128 };
		static constexpr XYi cGridSize{ 4096, 4096 };
		int32_t time{};	// frame number

		xx::Shared<Player> player;
		xx::List<xx::Shared<PlayerBullet>> playerBullets;
		Space<Monster> monsters;
		xx::Shared<Ground> ground;

		void Init() override;
		void Update() override;
		void Draw() override;
	};

}
