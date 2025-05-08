#pragma once

namespace Game {

	struct Monster_Roastduck : Monster {
		void Init(Stage* stage_, XY const& pos_);
		int32_t Update() override;
		void Rewards(Creature* target) override;
	};

}
