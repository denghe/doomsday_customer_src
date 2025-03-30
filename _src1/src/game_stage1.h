#pragma once

namespace Game {

	struct Stage1 : xx::SceneBase {
		xx::Camera camera;
		xx::Shared<xx::Node> ui;
		xx::Shared<Player> player;

		void Init() override;
		void Update() override;
		void Draw() override;
	};

}
