#pragma once

namespace Game {


	XX_INLINE bool PlayerWithAction::ActionExists(ActionTypes bt) {
		return (actionFlags & (1llu << (int32_t)bt)) > 0;
	}

	XX_INLINE void PlayerWithAction::ActionSetFlag(ActionTypes bt) {
		assert(!ActionExists(bt));
		actionFlags |= (1llu << (int32_t)bt);
	}

	XX_INLINE void PlayerWithAction::ActionClearFlag(ActionTypes bt) {
		assert(ActionExists(bt));
		actionFlags &= ~(1llu << (int32_t)bt);
	}

	// return -1 mean not found
	XX_INLINE int32_t PlayerWithAction::ActionFind(ActionTypes bt) {
		if (!ActionExists(bt)) return -1;
		for (int32_t i = actionsLen - 1; i >= 0; --i) {
			if (actions[i].type == bt) return i;
		}
		assert(false);
		return -1;
	}

	// call after ActionFind
	XX_INLINE void PlayerWithAction::ActionRemove(ActionTypes bt, int32_t index) {
		assert(ActionExists(bt));
		assert(index != -1);
		assert(index < _countof(actions));
		assert(actions[index].type == bt);
		actionFlags &= ~(1llu << (int32_t)bt);
		actions[index] = actions[--actionsLen];
	}

	// return false mean not found
	XX_INLINE bool PlayerWithAction::ActionTryRemove(ActionTypes bt) {
		if (!ActionExists(bt)) return false;
		for (int32_t index = actionsLen - 1; index >= 0; --index) {
			if (actions[index].type == bt) {
				ActionRemove(bt, index);
				return true;
			}
		}
		assert(false);
		return false;
	}

	template<typename T>
	XX_INLINE bool PlayerWithAction::ActionExists() {
		return ActionExists(T::cType);
	}

	template<typename T>
	XX_INLINE int32_t PlayerWithAction::ActionFind() {
		return ActionFind(T::cType);
	}

	template<typename T>
	XX_INLINE void PlayerWithAction::ActionRemove(int32_t index) {
		ActionRemove(T::cType, index);
	}

	template<typename T>
	XX_INLINE void PlayerWithAction::ActionRemove(T& o) {
		ActionRemove(T::cType, int32_t(((char*)&o - (char*)actions) / sizeof(Action)));
	}

	template<typename T>
	XX_INLINE void PlayerWithAction::ActionRemove(T&, int32_t index) {
		ActionRemove(T::cType, index);
	}

	template<typename T>
	XX_INLINE bool PlayerWithAction::ActionTryRemove() {
		return ActionTryRemove(T::cType);
	}

	template<typename...AS>
	XX_INLINE void PlayerWithAction::ActionTryRemoves() {
		xx::ForEachType<std::tuple<AS...>>([&]<typename T>() {
			ActionTryRemove(T::cType);
		});
	}

	template<typename T>
	XX_INLINE T& PlayerWithAction::ActionAdd() {
		assert(actionsLen < _countof(actions));
		assert(!ActionExists(T::cType));
		ActionSetFlag(T::cType);
		auto& o = (T&)actions[actionsLen++];
		o.type = T::cType;
		return o;
	}

	template<typename T>
	XX_INLINE T& PlayerWithAction::ActionAddOrRef() {
		if (!ActionExists(T::cType)) return ActionAdd<T>();
		auto index = ActionFind(T::cType);
		return (T&)actions[index];
	}

	XX_INLINE void PlayerWithAction::StepSpriteIndex(int32_t from, int32_t to, float step) {
		spriteIndex += step;
		while ((int32_t)spriteIndex > to) {
			spriteIndex = from + (spriteIndex - to);
		}
	}

	XX_INLINE int32_t PlayerWithAction::Add_Action_Walk() {
		if (ActionExists<Action_Walk>()) return 0;
		auto& o = ActionAdd<Action_Walk>();
		o.n = 0;
		return 1;
	}

	XX_INLINE int32_t PlayerWithAction::Add_Action_Atk1() {
		if (ActionExists<Action_Atk1>()) return 0;
		if (ActionExists<Action_Atk2>()) return -1;
		auto& o = ActionAdd<Action_Atk1>();
		o.n = 0;
		return 1;
	}

	XX_INLINE int32_t PlayerWithAction::Add_Action_Atk2() {
		if (ActionExists<Action_Atk2>()) return 0;
		if (ActionExists<Action_Atk1>()) return -1;
		auto& o = ActionAdd<Action_Atk2>();
		o.n = 0;
		return 1;
	}

	XX_INLINE void PlayerWithAction::Case_(Action_Walk& o) {
		if (ActionExists<Action_Atk1>() || ActionExists<Action_Atk2>()) return;
		XX_BEGIN(o.n);
		for (x = 0.f; x < 100.f; x += 100.f / 10.f / 60.f) {		// player use 10 seconds move
			StepSpriteIndex(0, 9, 10.f / 2.f / 60.f);				// 10 frame use 2 seconds play
			XX_YIELD(o.n);
		};
		for (x = 100.f; x > 0.f; x -= 100.f / 5.f / 60.f) {			// player use 5 seconds move back
			StepSpriteIndex(0, 9, 10.f / 2.f / 60.f);
			XX_YIELD(o.n);
		};
		XX_YIELD_TO_BEGIN(o.n);
		XX_END(o.n);
	}

	XX_INLINE void PlayerWithAction::Case_(Action_Atk1& o) {
		XX_BEGIN(o.n);
		spriteIndexBak = spriteIndex;
		spriteIndex = 10.f;
		do {
			XX_YIELD(o.n);
			spriteIndex += 5.f / 0.5f / 60.f;		// 5 frame play 0.5 seconds
		} while (spriteIndex < 15.f);
		spriteIndex = spriteIndexBak;
		ActionRemove(o);							// dispose self ( swap remove from actions )
		XX_END(o.n);
	}

	XX_INLINE void PlayerWithAction::Case_(Action_Atk2& o) {
		XX_BEGIN(o.n);
		spriteIndexBak = spriteIndex;
		spriteIndex = 15.f;
		do {
			XX_YIELD(o.n);
			spriteIndex += 5.f / 0.5f / 60.f;
		} while (spriteIndex < 20.f);
		spriteIndex = spriteIndexBak;
		ActionRemove(o);
		XX_END(o.n);
	}

	void PlayerWithAction::Update() {
		// execute all actions
		for (int32_t i = actionsLen - 1; i >= 0; --i) {
			auto& b = actions[i];
			switch (b.type) {
			case Action_Walk::cType: Case_((Action_Walk&)b); break;
			case Action_Atk1::cType: Case_((Action_Atk1&)b); break;
			case Action_Atk2::cType: Case_((Action_Atk2&)b); break;
			// ...
			}
		}
	}










	inline void Test1::MakeUI() {
		ui->Clear();
		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos7 + XY{ 10, -10 }
			, gLooper.anchor7, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});

		ui->MakeChildren<xx::Button>()->Init(1, gLooper.pos8 + XY{ 0, -10 }
			, gLooper.anchor8, gLooper.btnCfg, U"Add More Grass", [this]() {
				EnvGrass::GenGrass(this, 5);
			});
	}

	inline void Test1::OnWindowSizeChanged() {
		MakeUI();
	}

	inline void Test1::Init() {
		fb.Init();
		ui.Emplace()->Init();
		MakeUI();

		gridSize = {60, 60};
		mapSize = 128 * gridSize;

		ground.Emplace()->Init(this, mapSize, gLooper.res.ground_cell3);
		player.Emplace<Player_1>()->Init(this);

		EnvGrass::GenGrass(this, 5);

		camera.scale = Cfg::defaultScale;
		camera.mapSize = mapSize;
		camera.newOriginal = camera.original = mapSize * 0.5f;
	}

}
