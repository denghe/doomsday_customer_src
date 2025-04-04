#pragma once

namespace Game {
	template<typename T>
	int32_t MonsterGen_Generic<T>::Update() {
		//CreateFromRect<T>();
		CreateFromDoughnut<T>();
		return 0;
	}
}
