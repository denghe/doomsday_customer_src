#pragma once

namespace Game {

	void PlayerBullet_1::Init(PlayerSkill* ps) {
		pos = ps->ownerPlayer->pos;
		scale = 1;
		radians = M_PI_2;
		// todo
	}

	int32_t PlayerBullet_1::Update() {
		// todo
		return 0;
	}

	void PlayerBullet_1::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res.bullet_coin5->tex->GetValue(), 1);
		q->pos = ownerStage->camera.ToGLPos(pos);
		q->anchor = ResTpFrames::_anchor_bullet_coin5_;
		q->scale = scale * Cfg::globalScale * ownerStage->camera.scale;
		q->radians = radians;
		q->colorplus = 1;
		q->color = xx::RGBA8_White;
		q->texRect.data = gLooper.res.bullet_coin5->textureRect.data;
	}

}
