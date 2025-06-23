#pragma once

namespace Game {

	struct Player : Drawable {
		static constexpr auto cSize = XYi(ResTpFrames::_size_char_body);
		static constexpr XY cGravity{ 0.2, 0.4 };
		static constexpr XY cSpeedInit{ 0, -650 / Cfg::fps };
		static constexpr XY cSpeedMax{ 400 / Cfg::fps, 1000 / Cfg::fps };
		static constexpr XY cCenterPosOffset{ 0.f, -ResTpFrames::_size_char_body.y * 0.5f };
		static constexpr float cDownJumpYOffset{ 1 };
		static constexpr int32_t cCoyoteNumFrames{ int32_t(0.1 / Cfg::frameDelay) };
		static constexpr int32_t cHighJumpNumFrames{ int32_t(0.2 / Cfg::frameDelay) };

		XY _pos{};							// for anti-shake( std::floorf( pos )
		XY speed{};							// movement
		XY centerPos{};						// for monster aim
		float radius{};						// for monster bullet check
		int32_t lastXMoveDirection{};		// -1: left  0: stoped  1: right
		int32_t fallingFrameCount{};		// for coyote time
		int32_t highJumpFrameCount{};
		bool lastJumpPressed{};
		bool jumping{};
		bool highJumpStoped{};
		int32_t multiJumpedCount{};

		xx::Shared<PlayerWeapon> weapon;

		PlayerProperties pp;

		template<typename WT>
		void Init(struct Stage* stage_, int32_t bornPosIndex = 0);
		int32_t Update() override;
		void Draw() override;
		void DrawLight(float colorPlus_) override;

		std::pair<float, int> Hurt(float dp);
	};

}
