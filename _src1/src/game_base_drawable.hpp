#pragma once

namespace Game {

	inline void Drawable::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(frame->tex, 2);
		XY s{ scale * stage->camera.scale };
		if (needFlipX) s.x = -s.x;
		// body
		q[1].pos = stage->camera.ToGLPos(pos);
		q[1].anchor = frame->anchor.has_value() ? *frame->anchor : XY{ 0.5f, 0.5f };
		q[1].scale = s;
		q[1].radians = 0;
		q[1].colorplus = whiteColorEndTime >= stage->time ? 10000.f : 1.f;
		q[1].color = color;
		q[1].texRect.data = frame->textureRect.data;
		// shadow
		q[0].scale = { q[1].scale.x, q[1].scale.y * 0.2f };
		q[0].pos = { q[1].pos.x, q[1].pos.y - q[0].scale.y * frame->spriteSize.y * 0.5f };
		q[0].anchor = { q[1].anchor.x, 0.3 };
		q[0].radians = q[1].radians;
		q[0].colorplus = q[1].colorplus;
		q[0].color = { 0,0,0,127 };
		q[0].texRect.data = q[1].texRect.data;
	}

	XX_INLINE void Drawable::SetName(XY const& posOffset, std::u32string_view const& name_) {
		if (name == name_) return;
		name = name_;
		FillNameFrames(name_);
		namePosOffset = posOffset;
	}

	XX_INLINE void Drawable::SetName(XY const& posOffset, std::string_view const& name_) {
		SetName(posOffset, xx::StringU8ToU32(name_));
	}

	XX_INLINE void Drawable::FillNameFrames(std::u32string_view const& name_) {
		auto len = (int)name_.size();
		nameFrames.Resize(len);
		auto& ctc = gLooper.ctcDefault;
		nameSize = { 0, (float)ctc.canvasHeight };
		for (int i = 0; i < len; ++i) {
			auto& f = nameFrames[i];
			f = ctc.Find(name_[i]);
			nameSize.x += f.texRect.w;
		}
	}

	inline void Drawable::DrawNameBG() {
		auto anchorNS = nameSize * 0.5f;
		auto basePos = stage->camera.ToGLPos(pos + namePosOffset) + XY{ -anchorNS.x, anchorNS.y };
		auto& shader = gLooper.ShaderBegin(gLooper.shaderQuadInstance);
		// draw bg
		{
			static constexpr XY padding{ 5,2 };
			auto q = shader.Draw(gLooper.res._texid_pixel, 1);
			q->pos = basePos - padding;
			q->anchor = {};
			q->scale = (nameSize + padding * 2) / ResTpFrames::_size_pixel;
			q->radians = {};
			q->colorplus = 1;
			q->color = {0,0,0,122};
			q->texRect.data = ResTpFrames::_uvrect_pixel.data;
		}
	}

	inline void Drawable::DrawName() {
		auto anchorNS = nameSize * 0.5f;
		auto basePos = stage->camera.ToGLPos(pos + namePosOffset) + XY{ -anchorNS.x, anchorNS.y };
		auto& shader = gLooper.ShaderBegin(gLooper.shaderQuadInstance);
		for (auto& f : nameFrames) {
			auto& q = *shader.Draw(f.tex->GetValue(), 1);
			q.pos = basePos;
			q.anchor = {};
			q.scale = 1;
			q.radians = {};
			q.colorplus = 1;
			q.color = color;
			q.texRect.data = f.texRect.data;
			basePos.x += f.texRect.w;
		}
	}
}
