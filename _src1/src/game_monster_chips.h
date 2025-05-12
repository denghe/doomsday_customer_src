#pragma once

namespace Game {

	struct Monster_Chips : Monster {
		void Init(Stage* stage_, XY const& pos_);
		int32_t Update() override;
		void Rewards() override;
		void Draw() override;
		void DrawTag() override;
	};

	struct Chip_Loot : Loot {
		LootTypes type{};
		Stage* stage{};
		int32_t coin{};
		float damage{};
		float attackSpeed{};
		int32_t healthPoint{};

		void Init(Stage* stage_);
		void Collect(Creature* owner) override;
	};
}
