#pragma once

namespace Game {

	inline void Player::Init(Stage* stage_, int32_t bornPosIndex) {
		assert(!stage);
		stage = stage_;
		auto& blocks = stage->map->blocks;
		auto posIdx = stage->map->bornPlaces_Player[bornPosIndex];
		auto leftTopPos = blocks.cellSize * posIdx;
		pos.x = leftTopPos.x + blocks.cellSize * 0.5f;
		pos.y = leftTopPos.y + blocks.cellSize - 1.f;

		pp.Init();
		pp.CalcAll();

		weapon.Emplace<PlayerWeapon>()->Init(this, {0, -20});
	}

	XX_INLINE XYi Player::GetPosLT() {
		return { (int32_t)pos.x - (cSize.x >> 1), (int32_t)pos.y - cSize.y };
	}

	inline int32_t Player::Update() {

		// left right move command check
		int32_t moveDir;
		if (gLooper.KeyDown(xx::KeyboardKeys::A) && gLooper.KeyDown(xx::KeyboardKeys::D)) {
			if (lastXMoveDirection == -1) {
				moveDir = 1;
			}
			else if (lastXMoveDirection == 1) {
				moveDir = -1;
			}
			else {
				moveDir = 0;
			}
		}
		else if (gLooper.KeyDown(xx::KeyboardKeys::A)) {
			lastXMoveDirection = moveDir = -1;
		}
		else if (gLooper.KeyDown(xx::KeyboardKeys::D)) {
			lastXMoveDirection = moveDir = 1;
		}
		else {
			lastXMoveDirection = moveDir = 0;
		}

		// left right move
		if (moveDir == 1) {		// right
			if (speed.x < 0) speed.x = 0;
			speed.x += cGravity.x;
			if (speed.x > cSpeedMax.x) {
				speed.x = cSpeedMax.x;
			}
			pos.x += speed.x;
		}
		else if (moveDir == -1) {	// left
			if (speed.x > 0) speed.x = 0;
			speed.x -= cGravity.x;
			if (speed.x < -cSpeedMax.x) {
				speed.x = -cSpeedMax.x;
			}
			pos.x += speed.x;
		}
		else {
			speed.x = 0;
		}

		// handle gravity
		speed.y += cGravity.y;
		if (speed.y > cSpeedMax.y) {
			speed.y = cSpeedMax.y;
		}
		pos.y += speed.y;

		// prepare
		auto& blocks = stage->map->blocks;

		auto iPosLT = GetPosLT();
		auto iPosRB = iPosLT + cSize;

		// handle blocks
		PushOutWays pushOutWays{};
		auto criFrom = blocks.PosToColRowIndex(iPosLT);
		auto criTo = blocks.PosToColRowIndex(iPosRB - 1);
		// check cross & pushout
		for (int rowIdx = criFrom.y; rowIdx <= criTo.y; ++rowIdx) {
			for (int colIdx = criFrom.x; colIdx <= criTo.x; ++colIdx) {
				if (auto bc = blocks.TryAt({ colIdx, rowIdx }); bc) {
					if (bc->IsCross(iPosLT, cSize)) {
						auto [newPos, pushOutWay] = bc->PushOut(iPosLT, cSize);
						if (pushOutWay != PushOutWays::Unknown) {
							iPosLT = newPos;
							iPosRB = iPosLT + cSize;
							(uint32_t&)pushOutWays |= (uint32_t&)pushOutWay;
						}
					}
				}
			}
		}
		if ((uint32_t)pushOutWays) {
			pos.x = float(iPosLT.x + (cSize.x >> 1));
			pos.y = float(iPosLT.y + cSize.y);
			if (fallingFrameCount && ((uint32_t&)pushOutWays & (uint32_t)PushOutWays::Up) > 0) {
				lastJumpPressed = highJumpStoped = jumping = false;
				multiJumpedCount = fallingFrameCount = highJumpFrameCount = 0;
				speed.y = 0;
			}
			if (moveDir > 0 && ((uint32_t&)pushOutWays & (uint32_t)PushOutWays::Left) > 0) {
				speed.x = 0;
			}
			if (moveDir < 0 && ((uint32_t&)pushOutWays & (uint32_t)PushOutWays::Right) > 0) {
				speed.x = 0;
			}
		}

		// out of map check
		if (iPosLT.x < 0 || iPosLT.y < 0)
			return 1;
		if (iPosRB.x >= blocks.gridSize.x || iPosRB.y >= blocks.gridSize.y)
			return 1;

		// falling count
		if (speed.y > 0.f) {
			++fallingFrameCount;
		}

		// handle jump
		auto jumpPressed = gLooper.KeyDown(xx::KeyboardKeys::Space);
		auto downPressed = gLooper.KeyDown(xx::KeyboardKeys::S);
		auto downJumpPressed = jumpPressed && downPressed;
		auto highJumpPressed = jumpPressed && !downPressed && lastJumpPressed;
		auto firstJumpPressed = jumpPressed && !downPressed && !lastJumpPressed;
		if (!jumping) {
			if (firstJumpPressed && fallingFrameCount < cCoyoteNumFrames) {
				speed.y = cSpeedInit.y;
				fallingFrameCount = highJumpFrameCount = 0;
				jumping = true;
			}
			if (downJumpPressed && speed.y == 0) {
				pos.y += cDownJumpYOffset;
			}
		}
		else {
			if (firstJumpPressed && (multiJumpedCount < cExtraJumpCount)) {
				++multiJumpedCount;
				highJumpStoped = false;
				fallingFrameCount = highJumpFrameCount = 0;
				speed.y = cSpeedInit.y;
			}
			else if (highJumpPressed && !highJumpStoped) {
				++highJumpFrameCount;
				if (highJumpFrameCount < cHighJumpNumFrames) {
					speed.y = cSpeedInit.y;
				}
			}
			else {
				highJumpStoped = true;
			}
		}
		lastJumpPressed = jumpPressed;


		weapon->Update();

		return 0;
	}

	inline void Player::Draw() {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
			.Draw(gLooper.res._texid_char_body, 2);
		// body
		q[0].pos = stage->camera.ToGLPos(pos);
		q[0].anchor = { 0.5f, 0 };
		q[0].scale = stage->camera.scale;
		q[0].radians = 0;
		q[0].colorplus = 0.5f;
		q[0].color = xx::RGBA8_White;
		q[0].texRect.data = ResTpFrames::_uvrect_char_body.data;
		// head
		q[1].pos = q[0].pos;
		q[1].anchor = { 0.5f, 0 };
		q[1].scale = q[0].scale;
		q[1].radians = 0;
		q[1].colorplus = 0.7f;
		q[1].color = xx::RGBA8_White;
		q[1].texRect.data = ResTpFrames::_uvrect_char_head.data;

		//{
		//	auto q = gLooper.ShaderBegin(gLooper.shaderRingInstance).Draw(1);
		//	q->color = xx::RGBA8_White;
		//	q->pos = stage->camera.ToGLPos(pos) * XY { 1, gLooper.flipY };
		//	q->radius = 200;
		//}

		// weapon
		weapon->Draw();
	}

	inline void Player::DrawLight() {
		auto q = gLooper.ShaderBegin(gLooper.shaderRingInstance).Draw(1);
		q->pos = stage->camera.ToGLPos(pos);
		q->radius = Cfg::height * 0.8 * stage->camera.scale;
		q->color = {180,180,180,255};

		//auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance)
		//	.Draw(gLooper.res._texid_ef_light256, 1);
		//q[0].pos = stage->camera.ToGLPos(pos);
		//q[0].anchor = 0.5f;
		//q[0].scale = ResTpFrames::_size_char_body.x / ResTpFrames::_size_ef_light256.x * stage->camera.scale * 50.f;
		//q[0].radians = 0.f;
		//q[0].colorplus = 0.6f;
		//q[0].color = xx::RGBA8_White;
		//q[0].texRect.data = ResTpFrames::_uvrect_ef_light256.data;

		// weapon
		weapon->DrawLight();
	}
}
