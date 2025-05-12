#pragma once

namespace Game {
	
	struct Test5 : Stage {
		xx::Shared<UI_BuffShop> buffShop;
		xx::Shared<UI_BuffInfo> buffInfo;

		void Init() override;
		void DrawCustomUI() override;
	};

}
