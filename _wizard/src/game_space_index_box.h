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
	};

}
