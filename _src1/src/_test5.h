#pragma once

namespace Game {
	
	struct Test5 : Stage {
		xx::Shared<xx::Node> ui;
		xx::Shared<UI_BuffShop> buffShop;
		xx::Shared<UI_BuffInfo> buffInfo;

		void Init() override;
		void Update() override;
		void Draw() override;
	};

}
