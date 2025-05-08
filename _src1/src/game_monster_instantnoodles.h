#pragma once

namespace Game {

	struct Monster_Instantnoodles : Monster {
		void Init(Stage* stage_, XY const& pos_);
		void Rewards(Creature* target) override;
	};

}
