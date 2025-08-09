#pragma once

namespace Game {

	struct Test3;
	struct alignas(8) Test3Item {
		Test3* scene{};
		xx::XY pos{};
		float radius{};
		float radians{};
		virtual int32_t Update() { return 0; };
		virtual void Draw() {};
		virtual void DrawLight(float colorPlus_) {};
		virtual ~Test3Item() {};
	};

	// zuma algorithm snake ( body can hit, when body dead, shrink to tail )

	struct Pathway : xx::MovePathCache {
		// todo: move target funcs?
	};

	enum class SnakeElementTypes : int32_t {
		Unknown, Head, Body, Tail
	};

	struct Snake;
	struct SnakeElement : Test3Item {
		static constexpr float cSpeed{ 1.1f };
		static constexpr float cStickRatio{ 20.f };
		static constexpr float cRadiusAnimStep{ 0.1f };
		static constexpr xx::FromTo<float> cRadius{ 16.f, 32.f };

		// ********************************** fill by PreInit() **********************************
		Snake* owner{};
		SnakeElementTypes elementType{};
		int32_t index{ -1 };		// owner->elements[index] == this
		// ********************************** fill by PreInit() **********************************

		int32_t pathwayCursor{ -1 };

		void PreInit(Snake* owner_, SnakeElementTypes elementType_);
		void Init(float radius_);
		void Remove();				// remove from owner and sync all index
		bool HitCheck(XY p);
		SnakeElement* GetPrev();	// -> tail
		SnakeElement* GetNext();	// -> head
		int32_t BaseUpdate();
	};

	struct Snake : Test3Item {
		Test3* scene{};
		Pathway* pathway{};
		xx::Listi32<xx::Shared<SnakeElement>> elements;	// [0].elementType == Tail
		template<typename HeadType, typename BodyType, typename TailType>
		void Init(Test3* scene_, Pathway* pathway_, int32_t bodyLen_);
		int32_t Update() override;
		void Draw() override;
		void DrawLight(float colorPlus_) override;
	};

	struct SnakeHead : SnakeElement {
		void Init();
		int32_t Update() override;
		void Draw() override;
		void DrawLight(float colorPlus_) override;
	};

	struct SnakeTail : SnakeElement {
		void Init();
		int32_t Update() override;
		void Draw() override;
		void DrawLight(float colorPlus_) override;
	};

	struct SnakeBody : SnakeElement {
		int32_t U1_n{};
		void U1_RadiusAnim();

		void Init();
		int32_t Update() override;
		void Draw() override;
		void DrawLight(float colorPlus_) override;
	};

	struct Test3 : Scene {
		xx::Shared<xx::Node> ui;
		Pathway pathway;	// todo: list
		xx::Listi32<xx::Shared<Snake>> snakes;

		void MakeUI();

		void Init() override;
		void Update() override;
		void Draw() override;
		void OnWindowSizeChanged() override;
	};

}
