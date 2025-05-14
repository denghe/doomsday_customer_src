#pragma once

namespace Game {

	inline void Loot::Init(Creature* creature_) {
		stage = creature_->stage;
		frame = creature_->frame;
		pos = creature_->pos;
		scale = creature_->scale;
		needFlipX = creature_->needFlipX;
		creatureType = creature_->creatureType;

		coinValue = 5;
		auto rewardIdx = gLooper.rnd.Next(0, 3);

		switch (rewardIdx)
		{
		case 0:
			healthPoint = 10;
			break;
		case 1:
			damage = 1;
			break;
		case 2:
			attackSpeed = .5f;
			break;
		default:
			break;
		}

	}

	int32_t Loot::Update() {
		auto p = stage->player;
		auto pp = p->pos;
		auto d = pp - pos;
		auto dd = d.x * d.x + d.y * d.y;
		auto r2 = p->radius + radius;
		if (dd < r2 * r2) {
			Collect(p);
			return 1;
		}

		auto mag = std::sqrtf(dd);
		auto norm = d / mag;
		pos += norm * movementSpeedPerFrame;
		stage->ForceLimit(pos);
		return 0;
	}

	inline void Loot::Collect(Creature* owner) {
		if (coinValue) {
			owner->coin += coinValue;
		}

		switch (owner->creatureType)
		{
		case CreatureTypes::Player_Programmer:
			owner->UpdateHealthPoint(healthPoint);
			owner->UpdateAttackSpeed(attackSpeed);
			owner->UpdateDamage(damage);
		default:
			break;
		}

	}

	inline void Loot::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstanceOutline).Draw(frame, 2, xx::RGBA8_Green);
		XY s{ scale * stage->camera.scale * 0.8f };
		if (needFlipX) s.x = -s.x;
		q->pos = stage->camera.ToGLPos(pos);
		q->scale = s;
	}

	inline void Loot::DrawFlying() {
		Draw();
	}
}
