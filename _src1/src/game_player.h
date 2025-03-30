#pragma once

namespace Game {
	struct Stage1;
	struct Player {
		static constexpr float cMoveSpeed{ 300.f };	// pixel per seconds

		Stage1* owner{};
		xx::Frame* frame{};
		xx::XY pos{}, scale{};

		int32_t lineNumber_Anim_Idle{};
		void Anim_Idle();	// coroutine

		void Init(Stage1* owner_);
		void Update();
		void Draw();
	};

}
