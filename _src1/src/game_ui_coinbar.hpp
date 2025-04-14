#pragma once

namespace Game {

	inline void UI_CoinBar::Init(Stage* stage_) {
		stage = stage_;
		len = 1;
		buf[0] = '0';
	}

	inline UI_CoinBar& UI_CoinBar::SetValue(double coin_) {
		if (coin_ != coin) {
			len = xx::ToStringEN(coin_, buf.data());
		}
		return *this;
	}

	inline void UI_CoinBar::Draw() {
		auto& fs = gLooper.res.font_outline_;
		auto qs = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(fs[0]->tex, len);
		auto basePos = coinTextPos;
		for (int32_t i = 0; i < len; ++i) {
			auto& q = qs[i];
			q.pos = basePos;
			q.anchor = coinTextAnchor;
			q.scale = { coinCharScaleX, coinCharScaleY };
			q.radians = 0;
			q.colorplus = 1;
			q.color = xx::RGBA8_White;
			q.texRect.data = fs[buf[i] - 32]->textureRect.data;
			basePos.x += coinCharWidth;
		}
		{
			auto& q = *gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(gLooper.res._texid_ui_money, 1);
			q.pos = coinIconPos;
			q.anchor = coinIconAnchor;
			q.scale = coinIconScale;
			q.radians = 0;
			q.colorplus = 1;
			q.color = xx::RGBA8_White;
			q.texRect.data = gLooper.res._uvrect_ui_money.data;
		}
	}

}
