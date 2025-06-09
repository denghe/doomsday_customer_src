#pragma once

namespace Game {

	// stage's base
	struct Stage : Scene {

		Camera camera;
		xx::Rnd rnd;
		int32_t time{};
		float timePool{}, delta{};
		bool disableLight{};
		XY mapSize{};					// cache: map->blocks.gridSize

		int32_t roundId{};
		int32_t numReadyMonsters{};		// some logic counter

		int32_t _1n{}, _1a{}, _1b{};	// for KillAllMonster
		int32_t _2n{}, _2a{}, _2b{};	// for Update_

		xx::Listi32<xx::Shared<PlayerBullet>> playerBullets;
		SpaceIndexCircle<MonsterBullet, true> monsterBullets;
		xx::Shared<Player> player;
		SpaceIndexCircle<Monster, true> monsters;
		xx::Shared<Map> map;
		xx::Listi32<EffectExplosion> effectExplosions;
		EffectTextManager effectTexts;

		UI::Stage ui;

		void Init() override;
		void Update() override;
		void Draw() override;
		void OnWindowSizeChanged() override;

		void Update_();				// coroutine

		void ForceLimit(XY& pos);

		void SelectMapByRoundId();
		bool KillRandomMonster();	// return true: no monster for kill
		bool KillAllMonster();		// coroutine. return true: no monster for kill

		void UpdateAll();
		void UpdateCamera();
		void UpdateMap();
		void UpdatePlayer();
		void UpdatePlayerBullet();
		void UpdateMonsterBullet();
		void UpdateEffectExplosion();
		void UpdateEffectNumber();
		void UpdateMonster();
		// ...

	};

}
