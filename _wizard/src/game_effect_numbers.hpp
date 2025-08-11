#pragma once

namespace Game {

	void EffectText::Init(xx::XY const& pos_, xx::XY const& dist_, xx::RGBA8 color_, float scale_, int32_t value_, bool showSignal) {
		pos = pos_;
		// calculate move frame inc with random speed
		auto _1_mag = 1.f / std::sqrtf(dist_.x * dist_.x + dist_.y * dist_.y);
		inc = dist_ * _1_mag * gLooper.rnd.Next<float>(cMoveSpeedMin, cMoveSpeedMax);
		color = color_;
		alpha = 1;
		scale = scale_;

		std::string s;
		if (showSignal) {
			if (value_ >= 0) {
				s.push_back((char)58);
				xx::Append(s, (uint32_t)value_);
			}
			else {
				s.push_back((char)59);
				xx::Append(s, (uint32_t)-value_);
			}
		}
		else {
			xx::Append(s, (uint32_t)std::abs(value_));
		}
		auto buf = s.data();
		auto len = s.size();
		auto tar = (uint8_t*)&data.numbers;
		for (size_t i = 0; i < len; ++i) {
			tar[i] = buf[i] - 48;
		}
		tar[15] = (uint8_t)len;
		pos.x -= len * (13.f * 0.5f);
	}

	XX_INLINE int32_t EffectText::Update(Scene* scene) {
		XX_BEGIN(lineNumber);
		// move away
		timeout = scene->time + int32_t(cMoveDurationSeconds * Cfg::fps);
		while(scene->time <= timeout) {
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

	XX_INLINE void EffectText::Draw(Scene* scene) {
		data.pos = scene->camera.ToGLPos(pos);
		data.scale = scene->camera.scale * scale;
		data.color = { color.r, color.g, color.b, (uint8_t)(color.a * alpha) };
		gLooper.ShaderBegin(gLooper.shaderNumbers).Draw(data);
	}

	XX_INLINE void EffectTextManager::Init(Scene* scene_, int32_t cap) {
		scene = scene_;
		ens.Reserve(cap);
	}

	XX_INLINE void EffectTextManager::Add(xx::XY const& pos_, xx::XY const& dist_, xx::RGBA8 color_, float scale_, int32_t value_, bool showSignal) {
		ens.Emplace().Init(pos_, dist_, color_, scale_, value_, showSignal);
	}

	XX_INLINE bool EffectTextManager::Update() {
		int32_t n{};
		for (int32_t i = 0, e = ens.Count(); i < e; ++i) {
			if (ens[i].Update(scene)) {
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
