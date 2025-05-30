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
		xx::Rnd rnd;
		int32_t time{};

		// for monster generator
		int32_t _n{}, _a{}, _b{};

		// for monster generator

		xx::Listi32<xx::Shared<PlayerBullet>> playerBullets;	// todo: change to space index ?
		//xx::Listi32<xx::Shared<Bullet>> monsterBullets;
		xx::Shared<Player> player;
		//Space<Loot> loots;
		//xx::Listi32<xx::Shared<Loot>> flyingLoots;
		SpaceIndexCircle<Monster, true> monsters;
		//xx::Listi32<xx::Shared<Spawner>> spawners;
		xx::Shared<Map> map;
		//xx::Listi32<xx::Shared<Drawable>> effects;
		xx::Listi32<EffectExplosion> effectExplosions;

		void MonsterGen();

		void UpdateMap();
		void UpdatePlayer();
		void UpdatePlayerBullet();
		void UpdateEffectExplosion();
		void UpdateMonster();
		// ...

		void DrawLight_Circle(XY screenPos, XY radius = Cfg::unitRadius * 5, float colorPlus = 1.f, xx::RGBA8 color = xx::RGBA8_White);
	};

}
