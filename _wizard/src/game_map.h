#pragma once

namespace Game {

	struct Map {
		SpaceIndexBox<Block, false> blocks;
		xx::Listi32<XYi> bornPlaces_Monster;
		xx::Listi32<XYi> bornPlaces_Player;
		xx::Listi32<XYi> flyTargets_Monster;
		XY bgUvOffset{};

		std::unique_ptr<std::unique_ptr<uint8_t[]>[]> flowFields;	// todo
		uint8_t const* GetFlowField(XYi crIdx) const;

		void Init();
		void Update();
		void Draw(struct Stage* stage) const;
	};

}
