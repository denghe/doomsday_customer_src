#pragma once

namespace Game {

	// stage's base
	struct Stage : Scene {
		xx::Shared<xx::Node> ui;
		void MakeUI();

		void Init() override;
		void Update() override;
		void Draw() override;
		void OnWindowSizeChanged() override;

		xx::Camera camera;
		xx::Rnd rnd;							// for game logic only
		int32_t time{};							// for game logic only

		//xx::Listi32<xx::Shared<Bullet>> playerBullets;
		//xx::Listi32<xx::Shared<Bullet>> monsterBullets;
		xx::Shared<Player> player;
		//Space<Loot> loots;
		//xx::Listi32<xx::Shared<Loot>> flyingLoots;
		//Space<Monster> monsters;
		//xx::Listi32<xx::Shared<Spawner>> spawners;
		xx::Shared<Map> map;
		//xx::Listi32<xx::Shared<MonsterGen>> monsterGenerators;
		//xx::Listi32<xx::Shared<Drawable>> effects;
		//EffectTextManager effectTexts;
		template<bool updateTime = true>
		void StageUpdate();
		//template<bool clearPlayer, bool clearGrass>
		//void ClearItems();						// for round finished
		//// todo

		template<bool initMap = true>
		void StageInit();
		//virtual XY GetPlayerBornPos();
		//void ForceLimit(XY& pos);
		//bool IsOutOfMap(XY const& pos);
		//XY GetRndPosDoughnut(float maxRadius, float safeRadius);
		//void Update() override { StageUpdate(); };
		//virtual void DrawCustomOrderYItem(xx::Listi32<std::pair<float, Game::Drawable*>>& yd, XY areaMin, XY areaMax) {};
		//virtual void DrawCustomUI() {};
		//virtual void OnWindowSizeChanged() {};

		void DrawLight_Circle(XY screenPos, XY radius = Cfg::unitRadius * 5, float colorPlus = 1.f, xx::RGBA8 color = xx::RGBA8_White);
	};

}
