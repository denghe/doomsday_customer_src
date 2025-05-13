#pragma once

namespace Game {

	inline void Loot::Init(Creature* creature_, int32_t coinValue_) {
		stage = creature_->stage;
		frame = creature_->frame;
		pos = creature_->pos;
		scale = creature_->scale;
		needFlipX = creature_->needFlipX;
		creatureType = creature_->creatureType;
		coinValue = coinValue_;
		// todo
	}

	int32_t Loot::Update() {
		// todo: flying logic
		return 0;
	}

	inline void Loot::Collect(Creature* owner) {
		// todo: switch (creatureType) case
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
		// todo: trail ?
	}
}
