#pragma once

namespace Game {

	struct Player : Drawable {
		static constexpr auto cSize = XYi(ResTpFrames::_size_char_body);
		static constexpr xx::RGBA8 cLightColor{ xx::RGBA8_White };
		static constexpr float cLightRadiusRatio{ 1.f };
		static constexpr float cLightColorPlus{ 1.f };
		static constexpr XY cGravity{ 0.2, 0.4 };
		static constexpr XY cSpeedInit{ 0, -650 / Cfg::fps };
		static constexpr XY cSpeedMax{ 400 / Cfg::fps, 1000 / Cfg::fps };
		static constexpr float cDownJumpYOffset{ 1 };
		static constexpr int32_t cCoyoteNumFrames{ int32_t(0.1 / Cfg::frameDelay) };
		static constexpr int32_t cHighJumpNumFrames{ int32_t(0.2 / Cfg::frameDelay) };
		static constexpr int32_t cExtraJumpCount{ 1 };

		XY speed{};
		int32_t lastXMoveDirection{};		// -1: left  0: stoped  1: right
		int32_t fallingFrameCount{};		// for coyote time
		int32_t highJumpFrameCount{};
		bool lastJumpPressed{};
		bool jumping{};
		bool highJumpStoped{};
		int32_t multiJumpedCount{};

		xx::RGBA8 lightColor{};
		float lightColorPlus{};
		XY lightRadius{};
		xx::Shared<PlayerWeapon> weapon;

		PlayerProperties pp;

		XYi GetPosLT();
		void Init(struct Stage* stage_, int32_t bornPosIndex = 0);
		int32_t Update() override;
		void Draw() override;
		void DrawLight() override;
	};

}
