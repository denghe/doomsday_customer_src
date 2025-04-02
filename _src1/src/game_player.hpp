#pragma once

namespace Game {

	inline void Player_1::Init(Stage* stage_) {
		stage = stage_;
		pos = stage_->GetPlayerBornPos();
		scale = { 1,1 };
		radius = ResTpFrames::_size_player1_.x * 0.5f;
		radians = 0;

		hp = 100;
		damage = 1;
		moveSpeed = 500.f / Cfg::fps;
		criticalRate = 0.1f;
		criticalDamageRatio = 2;

		// add init skill for test
		skills.Emplace().Emplace<Skill_1>()->Init(this, stage->skillCfgs[0]);
	}

	inline int32_t Player_1::Update() {
		// move control
		auto posBak = pos;
		if (auto inc = gLooper.GetKeyboardMoveInc(); inc.has_value()) {
			pos += inc->second * moveSpeed;
			stage->ForceLimit(pos);	// for safe
			// todo: more block limit
		}
		auto moved = posBak == pos;

		// always
		Idle();

		// skills driven
		for (auto i = skills.len - 1; i >= 0; --i) {
			auto& skill = skills[i];
			if (skill->Update()) {
				skills.SwapRemoveAt(i);
			}
		}

		// if reutrn !0 mean player is dead
		return 0;
	}

	inline void Player_1::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res.player1->tex->GetValue(), 2);
		// body
		q[1].pos = stage->camera.ToGLPos(pos);
		q[1].anchor = ResTpFrames::_anchor_player1_;
		q[1].scale = scale * Cfg::globalScale * stage->camera.scale;
		q[1].radians = 0;
		q[1].colorplus = 1;
		q[1].color = xx::RGBA8_White;
		q[1].texRect.data = gLooper.res.player1->textureRect.data;
		// shadow
		q[0].scale = { q[1].scale.x, q[1].scale.y * 0.2f };
		q[0].pos = { q[1].pos.x, q[1].pos.y - q[0].scale.y * ResTpFrames::_size_player1_.y * 0.5f };
		q[0].anchor = { ResTpFrames::_anchor_player1_.x, 0.3 };
		q[0].radians = q[1].radians;
		q[0].colorplus = q[1].colorplus;
		q[0].color = { 0,0,0,127 };
		q[0].texRect.data = q[1].texRect.data;
	}
}
