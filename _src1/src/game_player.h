#pragma once

namespace Game {

	struct Player : Drawable {
		float moveSpeed{};
		float radius{};
		float damage{};
		float criticalRate{};
		float criticalDamageRatio{};
		xx::Listi32<xx::Shared<PlayerSkill>> skills;

		int32_t idle_lineNumber{};
		void Idle();	// coroutine

		void Init(StageBase* ownerStage_);
		void Update();
		void Draw() override;
	};

}
