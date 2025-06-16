#pragma once

namespace Game {

	inline void Test2::MakeUI() {
		ui.Emplace()->Init(0, {}, scale);
		ui->MakeChildren<xx::Button>()->Init(1, pos9 + XY{ -10, -10 }, anchor9
			, gLooper.btnCfg, U"exit", [&]() {
				gLooper.DelaySwitchTo<Game::MainMenu>();
			});
		ui->MakeChildren<xx::Button>()->Init(1, pos7 + XY{ 10, -10 }, anchor7
			, gLooper.btnCfg, U"A", [&]() {
				
			});
	}

	template<typename T>
	void Test2::SelectMonster() {
		mbs.Clear();
		auto o = xx::MakeShared<T>();
		o->Init(this);
		m = std::move(o);
	}


	inline void Test2::Init() {
		UpdateScale();
		MakeUI();
		SelectMonster<A>();
	}

	inline void Test2::Update() {

	}

	inline void Test2::Draw() {


		gLooper.DrawNode(ui);
	}

	inline void Test2::OnWindowSizeChanged() {
		MakeUI();
	}


	inline void A::Init(Test2* scene_) {
		scene = scene_;
	}
}
