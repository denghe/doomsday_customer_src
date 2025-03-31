#pragma once

namespace Game {

	struct Player : Drawable {
		static constexpr float cMoveSpeed{ 300.f };	// pixel per seconds

		int32_t lineNumber_Anim_Idle{};
		void Anim_Idle();	// coroutine

		void Init(StageBase* owner_);
		void Update();
		void Draw() override;
	};

}
