#pragma once

namespace Game {

	inline void Drawable::Draw() {
		//auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(frame->tex, 2);
		//XY s{ scale * stage->camera.scale };
		//if (needFlipX) s.x = -s.x;
		//// body
		//q[1].pos = stage->camera.ToGLPos(pos);
		//q[1].anchor = frame->anchor.has_value() ? *frame->anchor : XY{ 0.5f, 0.5f };
		//q[1].scale = s;
		//q[1].radians = radians;
		//q[1].colorplus = whiteColorEndTime >= stage->time ? 10000.f : 1.f;
		//q[1].color = color;
		//q[1].texRect.data = frame->textureRect.data;
		//// shadow
		//q[0].scale = { q[1].scale.x, -q[1].scale.y * 0.3f };
		//q[0].pos = q[1].pos;//{ q[1].pos.x, q[1].pos.y - q[0].scale.y * frame->spriteSize.y * 0.5f };
		//q[0].anchor = { q[1].anchor.x, 0 };
		//q[0].radians = -radians;
		//q[0].colorplus = q[1].colorplus;
		//q[0].color = { 0,0,0,127 };
		//q[0].texRect.data = q[1].texRect.data;
	}
}
