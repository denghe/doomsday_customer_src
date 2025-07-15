#pragma once

namespace Game {

	enum class PushOutWays : uint32_t {
		Unknown = 0, Up = 1, Right = 2, Down = 4, Left = 8
	};

	struct Block {
		XYi pos{}, size{};
		XYi drawPos{}, tiledIdx{};	// todo: DrawSize
		int32_t indexAtItems{ -1 }, indexAtCells{ -1 };
		xx::Math::BlockWayout wayout{};
		bool atEdge{}, isHalfSize{};

		xx::Shared<Block> Init(XYi pos_, XYi size_, XYi drawPos_, XYi tiledIdx_);
		void Update();
		void Draw(struct Stage* stage);

		template<typename T> void FillWayout(T& blocks);
		std::pair<XYi, PushOutWays> PushOutBox(XYi cPos, XYi cSize) const;
		bool PushOutCircle(XY& tarPos, float tarRadius);
		bool IsCrossBox(XYi cPosLT, XYi cSize) const;
	};

}
