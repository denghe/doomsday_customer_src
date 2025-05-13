#pragma once

namespace Game {

	struct Monster_Laptop : Monster {
		void Init(Stage* stage_, XY const& pos_);
		void Rewards() override;
	};

}
