#pragma once

namespace Game {

	inline xx::Shared<Block> Block::Init(XYi pos_, XYi size_) {
		pos = pos_;
		size = size_;
		return xx::SharedFromThis(this);
	}

	inline void Block::Update() {
	}

	inline void Block::Draw(Stage* stage) {
		auto q = gLooper.ShaderBegin(gLooper.shaderQuadInstance).Draw(gLooper.res._texid_ui_block, 1);
		q->pos = stage->camera.ToGLPos(pos);
		q->anchor = { 0, 1 };
		q->scale = XY{ size } * (1.f / gLooper.res._size_ui_block) * stage->camera.scale;
		q->radians = 0;
		q->colorplus = 1.f;
		q->color = xx::RGBA8_White;
		q->texRect.data = ResTpFrames::_uvrect_ui_block.data;
	}

	template<typename T>
	inline void Block::FillWayout(T& blocks) {
		// search neighbor & set wayout bit
		auto cri = blocks.PosToColRowIndex(pos);
		assert(blocks.TryAt(cri) == this);
		bool atEdge{};

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
			wayout.right = false;
		}
		else wayout.right = !blocks.TryAt({ cri.x + 1, cri.y });

		if (atEdge && !(uint8_t&)wayout) {
			if (cri.y != 0) wayout.up = true;
			if (cri.y + 1 != blocks.numRows) wayout.down = true;
			if (cri.x != 0) wayout.left = true;
			if (cri.x + 1 != blocks.numCols) wayout.right = true;
		}
	}

	// only for size >= cSize
	// cPos: left top position
	inline std::pair<XYi, PushOutWays> Block::PushOut(XYi const& cPos, XYi const& cSize) const {
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

	// cPos: left top position
	XX_INLINE bool Block::IsCross(XYi const& cPos, XYi const& cSize) const {
		if (cPos.x >= pos.x) {
			if (cPos.y >= pos.y) {
				if (cPos.x < pos.x + size.x) return cPos.y < pos.y + size.y;
			}
			else /* cPos.y < pos.y */ {
				if (cPos.x < pos.x + size.x) return cPos.y + cSize.y > pos.y;
			}
		}
		else /* cPos.x < pos.x */ {
			if (cPos.y >= pos.y) {
				if (cPos.x + cSize.x > pos.x) return cPos.y < pos.y + size.y;
			}
			else /* cPos.y < pos.y */ {
				if (cPos.x + cSize.x > pos.x) return cPos.y + cSize.y > pos.y;
			}
		}
		return false;
	}

}
