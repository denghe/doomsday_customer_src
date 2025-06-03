#pragma once

namespace Game {

	XX_INLINE void EffectText::Init(xx::XY const& pos_, xx::XY const& dist_, xx::RGBA8 color_, float scale_, double value_) {
		pos = pos_;
		// calculate move frame inc with random speed
		auto _1_mag = 1.f / std::sqrtf(dist_.x * dist_.x + dist_.y * dist_.y);
		inc = dist_ * _1_mag * gLooper.rnd.Next<float>(cMoveSpeedMin, cMoveSpeedMax);
		color = color_;
		alpha = 1;
		scale = scale_;

		auto s = xx::ToString((int32_t)(value_));
		auto buf = s.data();
		auto len = s.size();
		auto tar = (uint8_t*)&data.numbers;
		for (size_t i = 0; i < len; ++i) {
			tar[i] = buf[i] - 48;
		}
		tar[15] = (uint8_t)len;
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
		data.pos = stage->camera.ToGLPos(pos);
		data.scale = stage->camera.scale * scale;
		data.color = color;
		gLooper.ShaderBegin(gLooper.shaderNumbers).Draw(data);
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
