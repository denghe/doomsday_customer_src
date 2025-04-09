#pragma once

namespace Game {

	struct Test1 : Stage {
		xx::Shared<xx::Node> ui;
		xx::Shared<Player> player;

		void Init() override;
		void Update() override;
		void Draw() override;
	};

}
