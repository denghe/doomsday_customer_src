#pragma once

namespace Game {

	inline void Player::Knockback(float speed, XY const& d) {
		knockback = true;
		knockbackSpeed = speed * (1.f / Cfg::fps);
		auto dd = d.x * d.x + d.y * d.y;
		auto mag = std::sqrt(dd);
		knockbackDist = d / mag;
		knockbackReduceValuePerFrame = knockbackSpeed * (1.f / (Cfg::fps * 0.5f));
	}

	inline int32_t Player::Hurt() {
		if (stage->time < ProtectFrame) {
			xx::CoutN("无敌帧，受保护");
			return 0;
		}
		// todo  Hurt logic
		xx::CoutN("玩家受到伤害, 当前帧：",stage->time);
		ProtectFrame = stage->time + cProtectFrameCount;
		return 0;
	}

	inline int32_t Player::MoveToPosition(xx::XY targetPos, float targetRadius) {
		auto d = targetPos - pos;
		auto dd = d.x * d.x + d.y * d.y;
		auto r2 = targetRadius + radius;

		if (dd < r2 * r2) {
			return 1;
		}

		auto mag = std::sqrtf(dd);
		auto norm = d / mag;
		pos += norm * movementSpeedPerFrame;
		stage->ForceLimit(pos);
		return 0;
	}
}
