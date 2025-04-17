#pragma once

namespace Game {

	struct DashEarlyWarning : DrawableEx {
		xx::Scale9 s9;
		Creature* creature;
		int32_t n{};
		Stage* stage;
		xx::XY targetPos{};
		int32_t dashDelay{};

		void Init(Stage * stage_, Creature *creature_ ,xx::XY targetPos_, int32_t length_, int32_t dashDelay);
		int32_t Update() override;
		void Draw() override;
	};

}
