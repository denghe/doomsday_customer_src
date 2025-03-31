#pragma once

namespace Game {

	void Player::Idle() {
		XX_BEGIN(idle_lineNumber);
		for (scale.y = 1.f; scale.y >= IdleCfg::cEnd; scale.y -= IdleCfg::cInc) {
			scale.x = IdleCfg::cTotal - scale.y;
			XX_YIELD(idle_lineNumber);
		}
		for (; scale.y <= 1.f; scale.y += IdleCfg::cInc) {
			scale.x = IdleCfg::cTotal - scale.y;
			XX_YIELD(idle_lineNumber);
		}
		XX_YIELD_TO_BEGIN(idle_lineNumber);
		XX_END(idle_lineNumber);
	}

	inline void Player::Init(StageBase* ownerStage_) {
		ownerStage = ownerStage_;
		pos = ownerStage_->GetPlayerBornPos();
		scale = { 1,1 };
		moveSpeed = 300.f / Cfg::fps;
		radius = ResTpFrames::_size_player1_.x * 0.5f;

		damage = 1;
		criticalRate = 0.1f;
		criticalDamageRatio = 2;

		// add init skill for test
		skills.Emplace().Emplace<PlayerSkill_1>()->Init(this);
	}

	inline void Player::Update() {
		// move control
		auto posBak = pos;
		if (auto inc = gLooper.GetKeyboardMoveInc(); inc.has_value()) {
			pos += inc->second * moveSpeed;
			ownerStage->ForceLimit(pos);	// for safe
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
	}

	inline void Player::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res.player1->tex->GetValue(), 2);
		// body
		q[1].pos = ownerStage->camera.ToGLPos(pos);
		q[1].anchor = ResTpFrames::_anchor_player1_;
		q[1].scale = scale * Cfg::globalScale * ownerStage->camera.scale;
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
