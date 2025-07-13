#pragma once

namespace Game {

	struct Stage;
	struct Map : Drawable {
		SpaceIndexBox<Block, false> blocks;
		xx::Listi32<XYi> bornPlaces_Monster;
		xx::Listi32<XYi> bornPlaces_Player;
		xx::Listi32<XY> flyTargets_Monster;
		XY offset{};					// for GetFlyTargetPos
		XY bgUvOffset{};
		XY cameraOriginal{};
		int32_t _1n{}, _1a{}, _1b{};	// for GenerateMonster

		// flow fields for monster search player
		std::unique_ptr<std::unique_ptr<uint8_t[]>[]> flowFields;
		std::unique_ptr<uint32_t[]> tmp;
		std::unique_ptr<XYi[]> tmp2;
		XY GetFlowFieldVec(uint8_t* ds, XYi cri);
		XY GetFlowFieldVec(XY fromPos, float radius, XY tarPos);	// return move vec

		void Init(Stage* stage_, std::u32string_view mapText);
		int32_t Update() override;
		virtual void DrawBG();
		void Draw() override;
		void DrawLight(float colorPlus_) override;

		static XY IdxToPos(XYi idx);
		XY GetFlyTargetPos(int32_t idx);
		XY GetFlyTargetPosWithOffset(int32_t idx);
		void ShuffleFlyTargets();
		virtual int32_t GenerateMonster();
		template<typename M>
		int32_t GenerateMonster_core();
	};

	struct Map_1 : Map {
		void Init(Stage* stage_);
		int32_t Update() override;
		int32_t GenerateMonster() override;
	};

	struct Map_2 : Map {
		int32_t _n{};
		void Init(Stage* stage_);
		int32_t Update() override;
	};

	struct TiledMap : Map {
		static constexpr int32_t cTiledNumCols{ 6 };
		static constexpr int32_t cTiledNumRows{ 8 };
		static constexpr bool cTiledHalfSizeFlagss[cTiledNumCols * cTiledNumRows] = {
			1,1,1,1,1,1,
			0,0,0,0,0,0,
			0,0,0,0,0,0,
			1,1,1,0,0,0,
			0,0,1,1,0,0,
			0,0,0,0,0,0,
			0,0,1,0,0,0,
			0,0,0,0,1,0,
		};
	};

	struct Map_3 : TiledMap {
		void Init(Stage* stage_);
	};
}
