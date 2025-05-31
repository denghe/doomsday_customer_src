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

		Camera camera;
		xx::Rnd rnd;
		int32_t time{};
		float timePool{}, frameDelay{};
		XY mapSize{};					// cache: map->blocks.gridSize

		int32_t numReadyMonsters{};		// counter
		bool disableLight{};

		// for monster generator
		int32_t _n{}, _a{}, _b{};
		// for monster generator

		xx::Listi32<xx::Shared<PlayerBullet>> playerBullets;
		SpaceIndexCircle<MonsterBullet, true> monsterBullets;
		xx::Shared<Player> player;
		xx::Shared<MonsterFormation> monsterFormation;
		SpaceIndexCircle<Monster, true> monsters;
		xx::Shared<Map> map;
		xx::Listi32<EffectExplosion> effectExplosions;

		void ForceLimit(XY& pos);

		void MonsterGen();

		void UpdateCamera();
		void UpdateMap();
		void UpdateMonsterFormation();
		void UpdatePlayer();
		void UpdatePlayerBullet();
		void UpdateMonsterBullet();
		void UpdateEffectExplosion();
		void UpdateMonster();
		// ...
	};

}
