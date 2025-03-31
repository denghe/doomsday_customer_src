#pragma once

namespace Game {

	struct PlayerBullet;
	struct Player;
	struct Monster;
	struct Ground;

	struct StageBase : xx::SceneBase {
		xx::Camera camera;
		xx::Shared<xx::Node> ui;

		XYi gridSize{};	// grid's num cols rows
		XY mapSize{};	// cellSize * gridSize( grid pixel size )

		int32_t time{};	// frame number

		xx::Listi32<xx::Shared<PlayerBullet>> playerBullets;
		xx::Shared<Player> player;
		Space<Monster> monsters;
		xx::Shared<Ground> ground;

		virtual XY GetPlayerBornPos();
		void ForceLimit(XY& pos);
	};

	struct Drawable {
		StageBase* ownerStage{};
		xx::XY pos{}, scale{};
		virtual void Draw() {};
		virtual ~Drawable() {};
	};

	struct IdleCfg {
		static constexpr float cTotal{ 2.f };
		static constexpr float cRange{ 0.07f };
		static constexpr float cTimeSpan{ 0.3f };
		static constexpr float cInc{ cRange / (Cfg::fps * cTimeSpan) };
		static constexpr float cEnd{ 1.f - cRange };
	};

}
