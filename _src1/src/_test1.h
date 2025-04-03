#pragma once

namespace Game {

	struct Test1 : Stage {
		xx::Camera camera;
		xx::Shared<xx::Node> ui;
		xx::Listi32<xx::Shared<DrawableEx>> items;

		void Init() override;
		void Update() override;
		void Draw() override;
	};

}
