#pragma once

namespace Game {

	void Player::Anim_Idle() {
		static constexpr float cTotal{ 2.f };
		static constexpr float cRange{ 0.07f };
		static constexpr float cTimeSpan{ 0.3f };
		static constexpr float cInc{ cRange / (Cfg::fps * cTimeSpan) };
		static constexpr float cEnd{ 1.f - cRange };

		XX_BEGIN(anim_Idle_lineNumber);
		for (scale.y = 1.f; scale.y >= cEnd; scale.y -= cInc) {
			scale.x = cTotal - scale.y;
			XX_YIELD(anim_Idle_lineNumber);
		}
		for (; scale.y <= 1.f; scale.y += cInc) {
			scale.x = cTotal - scale.y;
			XX_YIELD(anim_Idle_lineNumber);
		}
		XX_YIELD_TO_BEGIN(anim_Idle_lineNumber);
		XX_END(anim_Idle_lineNumber);
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
	}

	inline void Player::Update() {
		if (auto inc = gLooper.GetKeyboardMoveInc(); inc.has_value()) {
			pos += inc->second * moveSpeed;
		}
		Anim_Idle();
	}

	inline void Player::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res.player1->tex->GetValue(), 1);
		q->pos = ownerStage->camera.ToGLPos(pos);
		q->anchor = ResTpFrames::_anchor_player1_;
		q->scale = scale * Cfg::globalScale * ownerStage->camera.scale;
		q->radians = 0;
		q->colorplus = 1;
		q->color = xx::RGBA8_White;
		q->texRect.data = gLooper.res.player1->textureRect.data;
	}
}
