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
		bool disableLight{};
		XY mapSize{};					// cache: map->blocks.gridSize

		int32_t roundId{};
		int32_t numReadyMonsters{};		// some logic counter

		int32_t _1n{}, _1a{}, _1b{};	// for MonsterGen
		int32_t _2n{}, _2a{}, _2b{};	// for Update_

		xx::Listi32<xx::Shared<PlayerBullet>> playerBullets;
		SpaceIndexCircle<MonsterBullet, true> monsterBullets;
		xx::Shared<Player> player;
		xx::Shared<MonsterFormation> monsterFormation;
		SpaceIndexCircle<Monster, true> monsters;
		xx::Shared<Map> map;
		xx::Listi32<EffectExplosion> effectExplosions;

		void Update_();

		void ForceLimit(XY& pos);

		void MonsterFormationInit();
		int32_t MonsterGen();
		bool KillRandomMonster();	// return true: no monster for kill
		bool KillAllMonster();		// return true: no monster for kill

		void UpdateAll();
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
