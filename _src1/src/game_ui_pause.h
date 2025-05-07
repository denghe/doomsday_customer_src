#pragma once

namespace Game {

	// modal panel
	struct UI_PausePanel {
		xx::Shared<xx::Node> ui;
		operator bool() const;
		void Init(Creature* creature_);
		void TryDraw();
	};

}
