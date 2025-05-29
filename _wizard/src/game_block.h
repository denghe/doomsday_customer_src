#pragma once

namespace Game {

	enum class PushOutWays : uint32_t {
		Unknown = 0, Up = 1, Right = 2, Down = 4, Left = 8
	};

	struct Block {
		XYi pos{}, size{};
		int32_t indexAtItems{ -1 }, indexAtCells{ -1 };
		xx::Math::BlockWayout wayout{};

		xx::Shared<Block> Init(XYi pos_, XYi size_);
		void Update();
		void Draw(struct Stage* stage);

		template<typename T> void FillWayout(T& blocks);
		std::pair<XYi, PushOutWays> PushOut(XYi const& cPos, XYi const& cSize) const;
		bool IsCross(XYi const& cPosLT, XYi const& cSize) const;
	};

}
