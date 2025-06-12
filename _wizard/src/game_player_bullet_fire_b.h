#pragma once

namespace Game {

	struct PlayerBullet_FireB : PlayerBullet {
		static constexpr xx::RGBA8 cLightColor{ 0xff,0xd1,0x54,0xff };
		static constexpr float cExplodeRadiusRatio{ 7.f };

		float frameIndex{};
		float radius{};
		float radians{};
		XY moveInc{};

		void Init(PlayerWeapon* shooter, XY pos_, float radians_);
		int32_t Update() override;
		void Draw() override;
		void DrawLight(float colorPlus_) override;

		bool Move();
		bool HitCheck();
	};

}
