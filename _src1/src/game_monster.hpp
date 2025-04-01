#pragma once

namespace Game {

	void Monster_1::Init(Stage* ownerStage_, XY const& pos_) {
		ownerStage = ownerStage_;
		pos = pos_;
		scale = 1;
		// todo
	}

	int32_t Monster_1::Update() {
		// todo
		return 0;
	}

	void Monster_1::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res.monster_chips->tex->GetValue(), 1);
		q->pos = ownerStage->camera.ToGLPos(pos);
		q->anchor = ResTpFrames::_anchor_monster_chips_;
		q->scale = scale * Cfg::globalScale * ownerStage->camera.scale;
		q->radians = M_PI_2;
		q->colorplus = 1;
		q->color = xx::RGBA8_White;
		q->texRect.data = gLooper.res.monster_chips->textureRect.data;
	}

}
