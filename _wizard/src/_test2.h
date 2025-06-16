#pragma once

namespace Game {

	struct Test2;
	struct M : Drawable {
		Test2* scene{};
	};
	struct MB : Drawable {
		Test2* scene{};
	};

	struct A : M {
		struct B : MB {

		};
		void Init(Test2* scene_);
	};
	
	struct Test2 : Scene {
		xx::Shared<xx::Node> ui;
		void MakeUI();
		xx::Shared<M> m;
		xx::Listi32<xx::Shared<MB>> mbs;

		template<typename T>
		void SelectMonster();

		void Init() override;
		void Update() override;
		void Draw() override;
		void OnWindowSizeChanged() override;
	};

}
