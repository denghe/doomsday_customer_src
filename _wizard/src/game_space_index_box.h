#pragma once
#include "game_space_index.h"

namespace Game {

	// space index with size limited box items
	// requires:
	/*
		XY pos{};
		XY size{};
		int32_t indexAtItems{ -1 }, indexAtCells{ -1 };
		Base *prev{}, *next{};		// if enableDoubleLink == true
	*/
	template<typename T, bool enableDoubleLink = false>
	struct SpaceIndexBox : SpaceIndex<T, enableDoubleLink> {
		// todo: more search funcs

		// 0 mean empty, 1 exists, 2 out of range
		int32_t ExistsPoint(XYi p) {
			if (p.x < 0 || p.y < 0 || p.x >= this->gridSize.x || p.y >= this->gridSize.y) return 2;
			if (this->At(this->PosToColRowIndex(p))) return 1;
			return 0;
		}

	};

}
