#pragma once

namespace Game {

	struct PlayerWeapon : Drawable {
		// todo
	};

	struct Player : Drawable {
		static constexpr XY cGravity{ 0.2, 0.4 };
		static constexpr XY cSpeedInit{ 0, -650 / Cfg::fps };
		static constexpr XY cSpeedMax{ 400 / Cfg::fps, 1000 / Cfg::fps };
		static constexpr float cDownJumpYOffset{ 1 };
		static constexpr int32_t cCoyoteNumFrames{ int32_t(0.1 / Cfg::frameDelay) };
		static constexpr int32_t cBigJumpNumFrames{ int32_t(0.2 / Cfg::frameDelay) };
		static constexpr bool cEnableStrictJumpMode{ true };

		XY lightRadius{};
		// todo: Weapon

		XY speed{};
		int32_t lastXMoveDirection{};		// -1: left  0: stoped  1: right
		int32_t fallingFrameCount{};		// for coyote time
		int32_t bigJumpFrameCount{};
		bool lastJumpPressed{};
		bool jumping{};
		bool doubleJumped{};
		bool longJumpStoped{};

		void Init(struct Stage* stage_, int32_t bornPosIndex = 0);
		int32_t Update() override;
		void Draw() override;
	};

}
