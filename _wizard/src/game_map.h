#pragma once

namespace Game {

	struct Map {
		SpaceIndexBox<Block, false> blocks;
		xx::Listi32<XYi> bornPlaces_Monster;
		xx::Listi32<XYi> bornPlaces_Player;

		void Init();
		void Update();
		void Draw(struct Stage* stage);
	};

}
