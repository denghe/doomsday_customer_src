#pragma once

namespace Game {

	XX_INLINE void StageItem::SetName(XY const& posOffset, std::u32string_view const& name_) {
		if (name == name_) return;
		name = name_;
		FillNameFrames(name_);
		namePosOffset = posOffset;
	}

	XX_INLINE void StageItem::SetName(XY const& posOffset, std::string_view const& name_) {
		SetName(posOffset, xx::StringU8ToU32(name_));
	}

	XX_INLINE void StageItem::FillNameFrames(std::u32string_view const& name_) {
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

	inline void StageItem::DrawNameBG() {
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

	inline void StageItem::DrawName() {
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
