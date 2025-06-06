#pragma once

namespace Game {

	struct PlayerBullet_FireA : PlayerBullet {
		static constexpr xx::RGBA8 cLightColor{ 0xff,0xd1,0x54,0xff };

		xx::Listi32<std::pair<xx::Weak<Drawable>, int32_t>> pierceBlackList;	// second: timeout

		float frameIndex{};
		float radius{};
		float radians{};
		XY moveInc{};

		void Init(PlayerWeapon* shooter, XY pos_, float radians_);
		int32_t Update() override;
		void Draw() override;
		void DrawLight() override;

		bool Move();
		bool HitCheck();
		void PlayDeathEffect(float scale_);
	};

}
