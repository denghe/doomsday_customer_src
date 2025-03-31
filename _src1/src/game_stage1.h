#pragma once

namespace Game {

	struct Stage1 : StageBase {
		xx::Shared<Player> player;
		xx::Shared<Ground> ground;

		void Init() override;
		void Update() override;
		void Draw() override;
	};

}
