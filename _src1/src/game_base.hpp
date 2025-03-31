#pragma once

namespace Game {

	XY StageBase::GetPlayerBornPos() {
		return mapSize * 0.5f;
	}

	void StageBase::ForceLimit(XY& pos) {
		if (pos.x < 0.f) pos.x = 0.f;
		else if (pos.x >= mapSize.x)
			pos.x = mapSize.x - 1;
		if (pos.y < 0.f) pos.y = 0.f;
		else if (pos.y >= mapSize.y)
			pos.y = mapSize.y - 1;
	}
}
