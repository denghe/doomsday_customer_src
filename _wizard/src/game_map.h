#pragma once

namespace Game {

	struct Map {
		SpaceIndexBox<Block, false> blocks;
		xx::Listi32<XYi> bornPlaces_Monster;
		xx::Listi32<XYi> bornPlaces_Player;
		xx::Listi32<XYi> flyTargets_Monster;
		XY bgUvOffset{};

		std::unique_ptr<std::unique_ptr<uint8_t[]>[]> flowFields;
		std::unique_ptr<uint32_t[]> tmp;
		std::unique_ptr<XYi[]> tmp2;
		XY GetFlowFieldVec(uint8_t* ds, XYi cri);
		XY GetFlowFieldVec(XY fromPos, float radius, XY tarPos);	// return move vec

		void Init();
		void Update();
		void Draw(struct Stage* stage);
	};

}
