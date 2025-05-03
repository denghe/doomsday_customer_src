#pragma once

namespace Game {
	
	/*********************************************************************************************/
	// action list enum

	enum class ActionTypes : int32_t {
		Walk,
		Atk1,
		Atk2,
		// ...
		__MaxValue__
	};
	static_assert((int32_t)ActionTypes::__MaxValue__ <= 64);

	/*********************************************************************************************/
	// base data struct

	struct alignas(8) Action {
		union {
			std::array<uint64_t, 1> _;	// need more size? increase it
			struct {
				ActionTypes type;
				int32_t __;
			};
		};
	};

	template<typename A>
	constexpr bool ActionStructCheck = alignof(Action) == alignof(A) && sizeof(A) <= sizeof(Action);

	/*********************************************************************************************/
	// action structs

	struct alignas(8) Action_Walk {
		static constexpr ActionTypes cType{ ActionTypes::Walk };
		ActionTypes type;
		int32_t n;
	};
	static_assert(ActionStructCheck<Action_Walk>);

	struct alignas(8) Action_Atk1 {
		static constexpr ActionTypes cType{ ActionTypes::Atk1 };
		ActionTypes type;
		int32_t n;
	};
	static_assert(ActionStructCheck<Action_Atk1>);

	struct alignas(8) Action_Atk2 {
		static constexpr ActionTypes cType{ ActionTypes::Atk2 };
		ActionTypes type;
		int32_t n;
	};
	static_assert(ActionStructCheck<Action_Atk2>);

	/*********************************************************************************************/
	// action user

	struct PlayerWithAction {
		int32_t actionsLen{};
		uint64_t actionFlags{};
		Action actions[2];											// todo: set more cap

		bool ActionExists(ActionTypes bt);							// return true: exists
		void ActionSetFlag(ActionTypes bt);
		void ActionClearFlag(ActionTypes bt);
		int32_t ActionFind(ActionTypes bt);							// return -1: not found
		void ActionRemove(ActionTypes bt, int32_t index);
		bool ActionTryRemove(ActionTypes bt);						// return -1: not found

		template<typename T> bool ActionExists();
		template<typename T> int32_t ActionFind();
		template<typename T> void ActionRemove(int32_t index);
		template<typename T> void ActionRemove(T& o);
		template<typename T> void ActionRemove(T&, int32_t index);	// maybe faster
		template<typename T> bool ActionTryRemove();
		template<typename...AS> void ActionTryRemoves();
		template<typename T> T& ActionAdd();
		template<typename T> T& ActionAddOrRef();

		float x{};						// player's position
		float spriteIndex{};			// 0 ~ 9: walk  10 ~ 14: atk1    15 ~ 19: atk2
		float spriteIndexBak{};

		void StepSpriteIndex(int32_t from, int32_t to, float step);

		int32_t Add_Action_Walk();
		int32_t Add_Action_Atk1();
		int32_t Add_Action_Atk2();
		// ...

		void Case_(Action_Walk& o);
		void Case_(Action_Atk1& o);
		void Case_(Action_Atk2& o);
		// ...

		void Update();
	};









	struct Grass : Drawable {
		static constexpr xx::FromTo<float> scaleRange{ 0.3f, 1.f };
		static constexpr xx::FromTo<float> swingRange{ -0.1f, 0.1f };
		float swingStep{};

		void Init(Stage* stage_);
		int32_t Update() override;
	};

	struct Test1 : Stage {
		void GenGrass();
		void Init() override;
		void Update() override;
	};

}
