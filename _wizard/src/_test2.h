#pragma once

namespace Game {

	struct Test2;
	struct alignas(8) Test2Item {
		Test2* scene{};
		xx::XY pos{};
		virtual int32_t Update() { return 0; };
		virtual void Draw() {};
		virtual void DrawLight(float colorPlus_) {};
		virtual ~Test2Item() {};
	};

	/************************************************************/

	struct M : Test2Item {
		float radius{}, radians{};
		float radiansCos{}, radiansSin{};	// cache for radians
		int32_t stunEndTime{};
		template<bool force = false> void SetRadians(float radians_);	// for fill cos sin
	};

	struct MB : Test2Item {
		float radius{}, radians{};
		void Draw() override;	// shadow
		virtual void Draw1();	// border
		virtual void Draw2();	// body
		virtual void Draw3();	// bone
	};

	/************************************************************/

	struct MB1;
	struct M1C {
		XY pos{};
		float radians{};
		// todo: more stat? scale?
	};

	struct M1 : M {
		static constexpr int32_t cShootNums{ 9 };
		static constexpr float cShootDelaySeconds{ 27.f / 120.f };
		static constexpr int32_t cShootDelayNumFrames{ int32_t(cShootDelaySeconds * Cfg::fps) };
		static constexpr float cShootBigDelaySeconds{ 180.f / 120.f };
		static constexpr int32_t cShootBigDelayNumFrames{ int32_t(cShootBigDelaySeconds * Cfg::fps) };
		static constexpr float cRadiansStep{ -float(M_PI) * 0.5f / ((cShootDelayNumFrames + 1) * cShootNums) };
		std::array<M1C, 4> cannonInfos;
		int32_t _n{}, _i{}, _t{}, _c{};
		void RotateTo(float radians_);	// SetRadians + SyncCannonInfos
		void SyncCannonInfos();
		void Shoot();
		void Init(Test2* scene_);
		int32_t Update() override;
		void Draw() override;
	};

	struct MB1 : MB {
		static constexpr float cRadius{ 40.f };
		static constexpr float cScaleDurationSeconds{ 27.f / 120.f };
		static constexpr int32_t cScaleDurationNumFrames{ int32_t(cScaleDurationSeconds * Cfg::fps) };
		static constexpr float c1_ScaleDurationNumFrames{ 1.f / cScaleDurationNumFrames };
		static constexpr float cMoveSpeedStep{ 50.f * c1_ScaleDurationNumFrames };
		xx::Weak<M1> shooter;
		int32_t _n{};
		int32_t cannonIndex{}, lifeEndTime{};
		float tarRadius{}, radiusInc{};
		XY posInc{};
		void Init(xx::Weak<M1> shooter_, int32_t cannonIndex_);
		int32_t Update() override;
	};

	/************************************************************/

	struct Test2 : Scene {

		Camera camera;
		xx::Rnd rnd;
		int32_t time{};
		float timePool{}, delta{};
		bool disableLight{};
		XY mapSize{};
		XY mousePos{};					// logic pos

		xx::Listi32<xx::Shared<M>> ms;
		xx::Listi32<xx::Shared<MB>> mbs;

		template<typename T>
		void SelectMonster();

		void Init() override;
		void Update() override;
		void Draw() override;
		void OnWindowSizeChanged() override;

		xx::Shared<xx::Node> ui;
		void MakeUI();
	};

}
