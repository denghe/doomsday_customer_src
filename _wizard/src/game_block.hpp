#pragma once

namespace Game {

	inline xx::Shared<Block> Block::Init(XYi pos_, XYi size_, XYi drawPos_, XYi tiledIdx_) {
		pos = pos_;
		size = size_;
		drawPos = drawPos_;
		tiledIdx = tiledIdx_;
		isHalfSize = size.x != size.y;
		return xx::SharedFromThis(this);
	}

	inline void Block::Update() {
	}

	inline void Block::Draw(Stage* stage) {
		auto f = gLooper.res_rocks.pointer;
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(f, 1);
		q->pos = stage->camera.ToGLPos(drawPos);
		q->anchor = { 0, 1 };
		q->scale = size.x * (1.f / 256.f) * stage->camera.scale;
		q->radians = 0;
		q->colorplus = 1.f;
		q->color = xx::RGBA8_White;
		q->texRect = { uint16_t(tiledIdx.x*256), uint16_t(tiledIdx.y*256), 256, 256 };
	}

	template<typename T>
	inline void Block::FillWayout(T& blocks) {
#if 0
		(uint8_t&)wayout = 0;
		static constexpr int dx{ 5 }, dy{ 5 };
		auto left = blocks.ExistsPoint(pos + XYi{ -dx, dy });
		auto up = blocks.ExistsPoint(pos + XYi{ dy, -dx });
		auto right = blocks.ExistsPoint(pos + size + XYi{ dx, -dy });
		auto down = blocks.ExistsPoint(pos + size + XYi{ -dy, dx });

		atEdge = left == 2 && up == 2
			|| up == 2 && right == 2
			|| right == 2 && down == 2
			|| down == 2 && left == 2;

		if (left > 0 && up > 0 && right > 0 && down > 0) {
			wayout.left = left == 1;
			wayout.up = up == 1;
			wayout.right = right == 1;
			wayout.down = down == 1;
		}
		else {
			wayout.left = left == 0;
			wayout.up = up == 0;
			wayout.right = right == 0;
			wayout.down = down == 0;
		}
#else
		auto cri = blocks.PosToColRowIndex(pos);
		assert(blocks.TryAt(cri) == this);

		if (cri.y == 0) {
			wayout.up = false; atEdge = true;
		}
		else wayout.up = !blocks.TryAt({ cri.x, cri.y - 1 });

		if (cri.y + 1 == blocks.numRows) {
			wayout.down = false; atEdge = true;
		}
		else wayout.down = !blocks.TryAt({ cri.x, cri.y + 1 });

		if (cri.x == 0) {
			wayout.left = false; atEdge = true;
		}
		else wayout.left = !blocks.TryAt({ cri.x - 1, cri.y });

		if (cri.x + 1 == blocks.numCols) {
			wayout.right = false; atEdge = true;
		}
		else wayout.right = !blocks.TryAt({ cri.x + 1, cri.y });

		if (atEdge && !(uint8_t&)wayout) {
			if (cri.y != 0) wayout.up = true;
			if (cri.y + 1 != blocks.numRows) wayout.down = true;
			if (cri.x != 0) wayout.left = true;
			if (cri.x + 1 != blocks.numCols) wayout.right = true;
		}
#endif
	}


	// only for size >= cSize
	// cPos: left top position
	inline std::pair<XYi, PushOutWays> Block::PushOutBox(XYi cPos, XYi cSize) const {
		// calculate 4 way distance & choose min val
		auto bPosRB = pos + size;	// RB: right bottom
		XYi bHalfSize{ size.x >> 1, size.y >> 1 };
		auto bCenter = pos + bHalfSize;
		auto cPosRB = cPos + cSize;
		auto cCenter = cPos + XYi{ cSize.x >> 1, cSize.y >> 1 };
		int32_t dLeft, dRight, dUp, dDown;
		if (cCenter.x >= bCenter.x) {
			if (wayout.left) dLeft = cPos.x - pos.x + cSize.x;
			else dLeft = 0x7FFFFFFF;
			if (wayout.right) dRight = bPosRB.x - cPos.x;
			else dRight = 0x7FFFFFFF;
			if (cCenter.y >= bCenter.y) {
				if (wayout.up) dUp = cPos.y - pos.y + cSize.y;
				else dUp = 0x7FFFFFFF;
				if (wayout.down) dDown = bPosRB.y - cPos.y;
				else dDown = 0x7FFFFFFF;
			}
			else {
				if (wayout.up) dUp = cPosRB.y - pos.y;
				else dUp = 0x7FFFFFFF;
				if (wayout.down) dDown = pos.y - cPos.y + cSize.y;
				else dDown = 0x7FFFFFFF;
			}
		}
		else {
			if (wayout.left) dLeft = cPosRB.x - pos.x;
			else dLeft = 0x7FFFFFFF;
			if (wayout.right) dRight = bPosRB.x - cPos.x;
			else dRight = 0x7FFFFFFF;
			if (cCenter.y >= bCenter.y) {
				if (wayout.up) dUp = cPos.y - pos.y + cSize.y;
				else dUp = 0x7FFFFFFF;
				if (wayout.down) dDown = bPosRB.y - cPos.y;
				else dDown = 0x7FFFFFFF;
			}
			else {
				if (wayout.up) dUp = cPosRB.y - pos.y;
				else dUp = 0x7FFFFFFF;
				if (wayout.down) dDown = pos.y - cPos.y + cSize.y;
				else dDown = 0x7FFFFFFF;
			}
		}
		if (dLeft == 0x7FFFFFFF && dRight == 0x7FFFFFFF && dUp == 0x7FFFFFFF && dDown == 0x7FFFFFFF)
			return { cPos, PushOutWays::Unknown };
		if (dRight <= dLeft && dRight <= dUp && dRight <= dDown) {
			if (dRight > bHalfSize.x) dRight = 1;		// protect
			return { { cPos.x + dRight, cPos.y }, PushOutWays::Right };
		}
		else if (dLeft <= dRight && dLeft <= dUp && dLeft <= dDown) {
			if (dLeft > bHalfSize.x) dLeft = 1;			// protect
			return { { cPos.x - dLeft, cPos.y }, PushOutWays::Left };
		}
		else if (dUp <= dLeft && dUp <= dRight && dUp <= dDown) {
			if (dUp > bHalfSize.y) dUp = 1;				// protect
			return { { cPos.x, cPos.y - dUp }, PushOutWays::Up };
		}
		else {
			if (dDown > bHalfSize.y) dDown = 1;			// protect
			return { { cPos.x, cPos.y + dDown }, PushOutWays::Down };
		}
	}

	XX_INLINE bool Block::PushOutCircle(XY& cPos, float cRadius) {
#if 0
		XY halfSize{ size.x * 0.5f, size.y * 0.5f };
		XY p{ pos.x + halfSize.x, pos.y + halfSize.y };
		return xx::Math::MoveCircleIfIntersectsBox<float>(wayout, p.x, p.y, halfSize.x, halfSize.y
			, cPos.x, cPos.y, cRadius, atEdge);
#else
		return xx::TranslateControl::MoveCircleIfIntersectsBox2({ pos, pos + size }, cPos, cRadius);
#endif
	}

	// cPos: left top position
	XX_INLINE bool Block::IsCrossBox(XYi cPosLT, XYi cSize) const {
		return xx::Math::IsAABBIntersect(pos, size, cPosLT, cSize);
	}

}
