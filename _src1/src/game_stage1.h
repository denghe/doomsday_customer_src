#pragma once

namespace Game {

	struct Stage1 : StageBase {
		xx::Shared<Player> player;
		xx::List<xx::Shared<PlayerBullet>> playerBullets;
		Space<Monster> monsters;
		xx::Shared<Ground> ground;

		void Init() override;
		void Update() override;
		void Draw() override;
	};

}
