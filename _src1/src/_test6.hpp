﻿#pragma once

namespace Game {


	template<int32_t len>
	XX_INLINE void Trail<len>::StepCursor() {
		++cursor;
		if (cursor == len) {
			cursor = 0;
		}
	}

	template<int32_t len>
	XX_INLINE Trail<len>::D& Trail<len>::At(int32_t idx) {
		auto i = cursor - idx;
		if (i < 0) {
			i += len;
		}
		return data[i];
	}

	template<int32_t len>
	void Trail<len>::Init(Stage* stage_, XY headPos_, float radius_) {
		assert(!stage);
		stage = stage_;
		cursor = 0;
		radius = radius_;
		for (int32_t i = 0; i < len; ++i) {
			data[i].first = headPos_;
			data[i].second = { 0, radius_ };
		}
	}

	template<int32_t len>
	void Trail<len>::Update(XY headPos_) {
		auto d = headPos_ - At(0).first;
		auto r = std::atan2f(d.y, d.x) + float(M_PI / 2);
		auto cos = std::cosf(r);
		auto sin = std::sinf(r);
		StepCursor();
		auto& head = At(0);
		head.first = headPos_;
		head.second = { cos * radius, sin * radius };
	}

	template<int32_t len>
	void Trail<len>::Draw() {

		auto texId = gLooper.res._texid_player1;
		static constexpr auto uvr = gLooper.res._uvrect_player1;

		auto [vi, vs, is] = gLooper.ShaderBegin(gLooper.shaderVerts).Draw(texId, 4 * (len - 1), 6 * (len - 1));

		static constexpr auto step = uvr.w / (float)(len - 1);
		for (int32_t i = len - 2; i >= 0; --i) {
			auto& curr = At(i);
			auto& next = At(i + 1);
			auto p1 = curr.first + curr.second;
			auto p2 = curr.first - curr.second;
			auto nextP1 = next.first + next.second;
			auto nextP2 = next.first - next.second;

			auto u1 = uint16_t(uvr.x + step * (len - i - 1));
			auto u2 = uint16_t(uvr.x + step * (len - i));
			vs[0] = { .pos = p1, .uv = {u1, uvr.y}, .color = xx::RGBA8_White};
			vs[1] = { .pos = nextP1, .uv = {u2, uvr.y}, .color = xx::RGBA8_White };
			vs[2] = { .pos = nextP2, .uv = {u2, uvr.y + uvr.h}, .color = xx::RGBA8_White };
			vs[3] = { .pos = p2, .uv = {u1, uvr.y + uvr.h}, .color = xx::RGBA8_White };

			is[0] = vi + 0;
			is[1] = vi + 1;
			is[2] = vi + 3;
			is[3] = vi + 1;
			is[4] = vi + 2;
			is[5] = vi + 3;

			vi += 4;
			vs += 4;
			is += 6;
		}
	}

	void Test6::Update() {
		Stage::Update();

		// trail logic
		for (auto& trail : trails) {
			trail.Update(gLooper.mouse.pos);
		}
	}




	inline void Test6::Init() {
		StageInit({60, 60});
		player.Emplace<Player_1>()->Init(this);

		ui.Emplace()->Init();
		MakeUI();

		// trail logic
		trails.Resize(1);// 10000
		for (auto& trail : trails) {
			trail.Init(this, gLooper.mouse.pos, 64);
		}
	}




	inline void Test6::MakeUI() {
		ui->Clear();
		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos7 + XY{ 10, -10 }
			, gLooper.anchor7, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});
	}

	inline void Test6::OnWindowSizeChanged() {
		MakeUI();
	}



	inline void Test6::DrawCustomUI() {
		// trail logic
		for (auto& trail : trails) {
			trail.Draw();
		}
	}
}
