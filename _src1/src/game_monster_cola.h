#pragma once

namespace Game {

	struct Monster_Cola : Monster {
		static constexpr float cAimRange{ 20.f };

		void Init(Stage* stage_, XY const& pos_);
		int32_t Update() override;
	};

}
