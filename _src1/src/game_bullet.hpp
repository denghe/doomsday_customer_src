#pragma once

namespace Game {

	inline void Bullet::Init(StageBase* ownerStage_, xx::Weak<Player> ownerPlayer_) {
		ownerStage = ownerStage_;
		ownerPlayer = std::move(ownerPlayer_);
	}

	inline void Bullet::Update() {
	}

	inline void Bullet::Draw() {
		//auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
		//	.Draw(gLooper.res.player1->tex->GetValue(), 1);
		//q->pos = ownerStage->camera.ToGLPos(pos);
		//q->anchor = ResTpFrames::_anchor_player1_;
		//q->scale = scale * Cfg::globalScale * ownerStage->camera.scale;
		//q->radians = 0;
		//q->colorplus = 1;
		//q->color = xx::RGBA8_White;
		//q->texRect.data = gLooper.res.player1->textureRect.data;
	}
}
