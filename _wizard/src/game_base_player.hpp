#pragma once

namespace Game {
	inline int32_t Player::Hurt() {
		if (stage->time < protectFrame) {
			xx::CoutN("protect Frame");
			return 0;
		}

		xx::CoutN("Player Hurt");
		protectFrame = stage->time + cProtectFrameCount;
		return 0;
	}
}
