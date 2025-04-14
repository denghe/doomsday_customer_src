#pragma once

namespace Game {

	inline void UI_HPBar::Init(Stage* stage_) {
		stage = stage_;
		s9.frame = gLooper.res.ui_hpbar;
		s9.center = hpbarCenter;
		s9.texScale = hpbarTexScale;
		s9.size = hpbarSize;
		s9.pos = hpbarPos;
	}

	inline UI_HPBar& UI_HPBar::SetValue(float hp_, float hpMax_) {
		assert(hpMax_ != 0);
		hp = hp_;
		hpMax = hpMax_;
		return *this;
	}

	inline void UI_HPBar::Draw() {
		// draw hp bar border
		s9.Draw<true, false>();
		// draw hp bar content
		XY s{ hpbarContentScale.x * (hp / hpMax) , hpbarContentScale.y };
		auto& q = *gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(gLooper.res._texid_pixel, 1);
		q.pos = hpbarContentPos;
		q.anchor = {};
		q.scale = s;
		q.radians = 0;
		q.colorplus = 1;
		q.color = hpbarContentColor;
		q.texRect = { ResTpFrames::_uvrect_pixel };
	}

}
