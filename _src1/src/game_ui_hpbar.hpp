#pragma once

namespace Game {

	inline void UI_HPBar::Init() {
		s9.frame = gLooper.res.ui_hpbar;
		s9.center = hpbarCenter;
		s9.texScale = hpbarTexScale;
		s9.size = hpbarSize;
		s9.pos = hpbarPos;
	}

	inline void UI_HPBar::Draw() {
		// draw hp bar border
		s9.Draw();
		// draw hp bar content
		XY s{ hpbarContentScale.x * (hp / hpMax) , hpbarContentScale.y };
		auto& q = *gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(gLooper.res._texid_ui_hpbar_content, 1);
		q.pos = hpbarContentPos;
		q.anchor = {};
		q.scale = s;
		q.radians = 0;
		q.colorplus = 1;
		q.color = xx::RGBA8_White;
		q.texRect = { ResTpFrames::_uvrect_ui_hpbar_content };
	}

}
