#pragma once

namespace Game {

	XX_INLINE void EffectText::Init(xx::XY const& pos_, xx::XY const& dist_, xx::RGBA8 color_, float scale_, double value_) {
		// calculate center point
		pos = { pos_.x - ResTpFrames::_size_font_outline_48.x * scale_ * len * 0.5f, pos_.y };
		// calculate move frame inc with random speed
		auto _1_mag = 1.f / std::sqrtf(dist_.x * dist_.x + dist_.y * dist_.y);
		inc = dist_ * _1_mag * gLooper.rnd.Next<float>(cMoveSpeedMin, cMoveSpeedMax);
		color = color_;
		alpha = 1;
		scale = scale_;
		//std::numeric_limits<double>::min()
		len = xx::ToStringEN(value_, buf.data());
	}

	XX_INLINE int32_t EffectText::Update(Stage* stage) {
		XX_BEGIN(lineNumber);
		// move away
		timeout = stage->time + int32_t(cMoveDurationSeconds * Cfg::fps);
		while(stage->time <= timeout) {
			pos += inc;
			XX_YIELD_I(lineNumber);
		}
		// fade out
		for (; alpha >= 0; alpha -= cFadeOutStep) {
			XX_YIELD_I(lineNumber);
		}
		alpha = 0;
		XX_END(lineNumber);
		return 1;
	}

	XX_INLINE void EffectText::Draw(Stage* stage) {
		auto& fs = gLooper.res.font_outline_;
		auto qs = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(fs[0]->tex, len);
		auto basePos = stage->camera.ToGLPos(pos);
		auto s = stage->camera.scale * scale;
		auto widthInc = ResTpFrames::_size_font_outline_48.x * s;
		basePos.x -= widthInc * len / 2;
		for (int32_t i = 0; i < len; ++i) {
			auto& q = qs[i];
			q.pos = basePos;
			q.anchor = { 0, 0.5f };
			q.scale = s;
			q.radians = 0;
			q.colorplus = 1;
			q.color = {color.r, color.g, color.b, (uint8_t)(color.a * alpha)};
			q.texRect.data = fs[buf[i] - 32]->textureRect.data;
			basePos.x += widthInc;
		}
	}

	XX_INLINE void EffectTextManager::Init(Stage* stage_, int32_t cap) {
		stage = stage_;
		ens.Reserve(cap);
	}

	XX_INLINE void EffectTextManager::Add(xx::XY const& pos_, xx::XY const& dist_, xx::RGBA8 color_, float scale_, double value_) {
		ens.Emplace().Init(pos_, dist_, color_, scale_, value_);
	}

	XX_INLINE bool EffectTextManager::Update() {
		int32_t n{};
		for (int32_t i = 0, e = ens.Count(); i < e; ++i) {
			if (ens[i].Update(stage)) {
				++n;
			}
		}
		ens.PopMulti(n);
		return ens.Empty();
	}

	XX_INLINE void EffectTextManager::Clear() {
		ens.Clear();
	}

};
