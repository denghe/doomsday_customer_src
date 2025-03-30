#pragma once

namespace Game {
	struct Stage1;
	struct Player {
		Stage1* owner{};
		xx::Frame* frame{};
		xx::XY pos{};

		float sx{}, sy{};
		int32_t lineNumber{};
		void Anim_Idle();

		void Init(Stage1* owner_);
		void Update();
		void Draw();
	};

}
