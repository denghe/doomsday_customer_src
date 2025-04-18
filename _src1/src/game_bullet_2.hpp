#pragma once

namespace Game {

	void Bullet_2::Init(Skill_Shoot_1* skill, XY const& pos_, float radians_, float lifeSeconds_, float cos, float sin) {
		stage = skill->creature->stage;
		frame = gLooper.res.bullet_coin5;

		moveSpeed = skill->moveSpeed;
		radius = skill->radius;
		damage = skill->damage;
		damageRatio = skill->creature->sp.damageRatio;
		criticalChance = skill->creature->sp.criticalChance;
		criticalBonusRatio = skill->creature->sp.criticalBonusRatio;

		pos = pos_;
		scale = radius / ResTpFrames::_size_bullet_coin5.x;
		radians = radians_;

		lifeEndTime = stage->time + int32_t(lifeSeconds_ * Cfg::fps);
		inc = { cos * moveSpeed, sin * moveSpeed };
	}

	int32_t Bullet_2::Update() {
		pos += inc;
		if (stage->IsOutOfMap(pos)) return -1;
		auto player = stage->player;
		auto playerPos = player->pos;
		auto d = playerPos - pos;
		auto dd = d.x * d.x + d.y * d.y;
		auto r2 = player->radius + radius;
		if (dd < r2 * r2) {
			return 1;
		}
        return lifeEndTime < stage->time;
	}

	void Bullet_2::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res._texid_bullet_coin5, 1);
		q->pos = stage->camera.ToGLPos(pos);
		q->anchor = ResTpFrames::_anchor_bullet_coin5;
		q->scale = scale * stage->camera.scale;
		q->radians = radians;
		q->colorplus = 1;
		q->color = xx::RGBA8_White;
		q->texRect.data = ResTpFrames::_uvrect_bullet_coin5.data;
	}

}
