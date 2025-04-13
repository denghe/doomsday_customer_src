#pragma once

namespace Game {
	
	struct Test2 : Stage {
		xx::Shared<xx::Node> ui;
		xx::Scale9 s9;
		float centerScaleY{};

		int32_t n1{}, n2{}, n3{};
		void Update1();
		void Update2();
		void Update3();

		void Init() override;
		void Update() override;
		void Draw() override;
	};

}
