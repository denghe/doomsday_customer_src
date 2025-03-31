#pragma once

namespace Game {

	struct Player;
	struct Bullet : Drawable {
		static constexpr float cMoveSpeed{ 300.f };	// pixel per seconds
		xx::Weak<Player> ownerPlayer{};

		void Init(StageBase* ownerStage_, xx::Weak<Player> ownerPlayer_);	// todo: more args
		void Update();
		void Draw() override;
	};

}
