#pragma once

namespace Game {

	struct Player;

	struct PlayerBullet : Drawable {
		xx::Weak<Player> ownerPlayer{};
		//SkillConfig cfg;

		void BaseInit(StageBase* ownerStage_, xx::Weak<Player> ownerPlayer_);	// todo: more args
		virtual int32_t Update() { return 0; }	// need override
	};

	struct PlayerBullet_1 : PlayerBullet {
		// todo: more args
		void Init(StageBase* ownerStage_, xx::Weak<Player> ownerPlayer_);
		int32_t Update() override;
		void Draw() override;
	};

	// ...

}
