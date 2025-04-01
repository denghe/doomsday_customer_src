#pragma once

namespace Game {

	void PlayerBullet_1::Init(Skill* skill, XY const& pos_, float radians_, float scale_, float cos, float sin) {
		stage = skill->player->stage;
		cfg = skill->cfg;

		pos = pos_;
		scale = scale_;
		radians = radians_;

		lifeEndTime = stage->time + cfg->life;
		inc = { cos * cfg->moveSpeed, sin * cfg->moveSpeed };
	}

	int32_t PlayerBullet_1::Update() {
		pos += inc;
		if (stage->IsOutOfMap(pos)) return -1;
		if (auto m = stage->monsters.FindFirstCrossBy9(pos.x, pos.y, cfg->radius)) {
			m->Hurt(this);
			return 1;
		}
        return lifeEndTime < stage->time;
	}

	void PlayerBullet_1::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res.bullet_coin5->tex->GetValue(), 1);
		q->pos = stage->camera.ToGLPos(pos);
		q->anchor = ResTpFrames::_anchor_bullet_coin5_;
		q->scale = scale * Cfg::globalScale * stage->camera.scale;
		q->radians = radians;
		q->colorplus = 1;
		q->color = xx::RGBA8_White;
		q->texRect.data = gLooper.res.bullet_coin5->textureRect.data;
	}

}
