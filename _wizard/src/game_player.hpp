#pragma once

namespace Game {

	// todo: weapon


	inline void Player::Init(Stage* stage_, int32_t bornPosIndex) {
		assert(!stage);
		stage = stage_;
		auto& blocks = stage->map->blocks;
		auto posIdx = stage->map->bornPlaces_Player[bornPosIndex];
		auto leftTopPos = blocks.cellSize * posIdx;
		pos.x = leftTopPos.x + blocks.cellSize * 0.5f;
		pos.y = leftTopPos.y + blocks.cellSize - 1.f;

		lightColor = xx::RGBA8_White;
		lightRadius = ResTpFrames::_size_char_body * 0.5f * 10.f;

		weapon.Emplace<PlayerWeapon>()->Init(this, {0, -20});
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
		static constexpr auto size = XYi(gLooper.res._size_char_body);

		auto iPosRB = pos.As<int32_t>();
		iPosRB.x += size.x >> 1;
		auto iPosLT = iPosRB - size;

		// handle blocks
		PushOutWays pushOutWays{};
		auto criFrom = blocks.PosToColRowIndex(iPosLT);
		auto criTo = blocks.PosToColRowIndex(iPosRB - 1);
		// check cross & pushout
		for (int rowIdx = criFrom.y; rowIdx <= criTo.y; ++rowIdx) {
			for (int colIdx = criFrom.x; colIdx <= criTo.x; ++colIdx) {
				if (auto bc = blocks.TryAt({ colIdx, rowIdx }); bc) {
					if (bc->IsCross(iPosLT, size)) {
						auto [newPos, pushOutWay] = bc->PushOut(iPosLT, size);
						if (pushOutWay != PushOutWays::Unknown) {
							iPosLT = newPos;
							iPosRB = iPosLT + size;
							(uint32_t&)pushOutWays |= (uint32_t&)pushOutWay;
						}
					}
				}
			}
		}
		if ((uint32_t)pushOutWays) {
			pos.x = float(iPosLT.x + (size.x >> 1));
			pos.y = float(iPosLT.y + size.y);
			if (fallingFrameCount && ((uint32_t&)pushOutWays & (uint32_t)PushOutWays::Up) > 0) {
				longJumpStoped = doubleJumped = jumping = false;
				fallingFrameCount = bigJumpFrameCount = 0;
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
		auto longJumpPressed = jumpPressed && !downPressed && lastJumpPressed;
		auto firstJumpPressed = jumpPressed && !downPressed && !lastJumpPressed;
		if (!jumping) {
			if (firstJumpPressed && fallingFrameCount < cCoyoteNumFrames) {
				speed.y = cSpeedInit.y;
				fallingFrameCount = bigJumpFrameCount = 0;
				jumping = true;
			}
			if (downJumpPressed && speed.y == 0) {
				pos.y += cDownJumpYOffset;
			}
		}
		else {
			if (firstJumpPressed && !doubleJumped) {
				doubleJumped = true;
				longJumpStoped = false;
				fallingFrameCount = bigJumpFrameCount = 0;
				speed.y = cSpeedInit.y;
			}
			else if (longJumpPressed && !longJumpStoped) {
				++bigJumpFrameCount;
				if (bigJumpFrameCount < cBigJumpNumFrames) {
					speed.y = cSpeedInit.y;
				}
			}
			else {
				if constexpr (cEnableStrictJumpMode) {
					longJumpStoped = true;
				}
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
		q[0].colorplus = 1.f;
		q[0].color = xx::RGBA8_White;
		q[0].texRect.data = ResTpFrames::_uvrect_char_body.data;
		// head
		q[1].pos = q[0].pos;
		q[1].anchor = { 0.5f, 0 };
		q[1].scale = q[0].scale;
		q[1].radians = 0;
		q[1].colorplus = 1.f;
		q[1].color = xx::RGBA8_White;
		q[1].texRect.data = ResTpFrames::_uvrect_char_head.data;
		// weapon
		weapon->Draw();
	}
}
