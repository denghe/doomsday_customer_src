#pragma once

namespace Game {

	// predef
	struct Bullet;
	struct Player;
	struct Ground;
	struct Monster;
	struct Drawable;
	struct Creature;

	// stage's base
	struct Stage : Scene {
		xx::Camera camera;
		xx::Rnd rnd;							// for game logic only
		int32_t time{};							// for game logic only

		//xx::Listi32<xx::Shared<Bullet>> playerBullets;
		//xx::Listi32<xx::Shared<Bullet>> monsterBullets;
		//xx::Shared<Player> player;
		//Space<Loot> loots;
		//xx::Listi32<xx::Shared<Loot>> flyingLoots;
		//Space<Monster> monsters;
		//xx::Listi32<xx::Shared<Spawner>> spawners;
		//xx::Shared<Ground> ground;
		//xx::Listi32<xx::Shared<MonsterGen>> monsterGenerators;
		//xx::Listi32<xx::Shared<Drawable>> effects;
		//EffectTextManager effectTexts;
		//template<bool updateTime = true>
		//void StageUpdate();
		//template<bool clearPlayer, bool clearGrass>
		//void ClearItems();						// for round finished
		//// todo

		//template<bool initGround = true>
		//void StageInit(XYi gridSize_);
		//virtual XY GetPlayerBornPos();
		//void ForceLimit(XY& pos);
		//bool IsOutOfMap(XY const& pos);
		//XY GetRndPosDoughnut(float maxRadius, float safeRadius);
		//void Update() override { StageUpdate(); };
		//virtual void DrawCustomOrderYItem(xx::Listi32<std::pair<float, Game::Drawable*>>& yd, XY areaMin, XY areaMax) {};
		//virtual void DrawCustomUI() {};
		//virtual void OnWindowSizeChanged() {};
		//void Draw() override;
	};

}
