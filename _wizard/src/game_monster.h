#pragma once

namespace Game {

	struct Monster : Drawable {
		static constexpr xx::RGBA8 cLightColor{ 255,127,127,255 };	// red
		static constexpr float cLightRadiusRatio{ 1.f };
		static constexpr float cLightColorPlus{ 0.5f };
		static constexpr float cMoveSpeed{ 500.f / Cfg::fps };
		static constexpr float cShootDelay{ 1.f };
		static constexpr XY cShootOffset{ 0.5f, 0 };	// ratio

		int32_t indexAtItems{ -1 }, indexAtCells{ -1 };
		Monster* prev{}, * next{};

		xx::Ref<xx::Frame> frame;
		xx::Weak<MonsterFormation> monsterFormation;
		int32_t monsterFormationPosIdx{};

		float radius{};
		float radians{};

		float shootTimePool{};
		int32_t _n{};

		xx::RGBA8 lightColor{};
		float lightColorPlus{};
		XY lightRadius{};

		bool FlyToTarget();
		void HandleBlock();
		void SyncPos();
		void FaceToPlayer();
		void TryShoot();

		void Init(Stage* stage_, xx::Ref<xx::Frame> frame_, int32_t bornPosIdx
			, xx::Weak<MonsterFormation> monsterFormation_, int32_t monsterFormationPosIdx_);
		int32_t Update() override;
		void Draw() override;
	};

}
