#pragma once

namespace Game {

	struct Monster_Hamburger : Monster {
		void Init(Stage* stage_, XY const& pos_);
		void Rewards() override;
	};

}
