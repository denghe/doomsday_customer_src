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

namespace PlatformGame {

	struct Scene;
	struct SceneItem {
		Scene* scene{};
		XY pos{}, size{};
		virtual int32_t Update() { return 0; };
		virtual void Draw() {};
		virtual ~SceneItem() {}
	};

	struct CharacterContainer;
	struct Character : SceneItem {
		CharacterContainer* attachedTo{};
		void AttachTo(CharacterContainer* tar);
		int32_t Update() override;
	};

	struct Player : Character {
	};

	struct Monster : Character {
	};

	struct CharacterContainer : SceneItem {
		xx::Listi32<xx::Weak<Character>> attachedCharacters;
		virtual void Handle(Character* c) {};
		// enter leave xxxx
	};

	struct Block : CharacterContainer {
		void Handle(Character* c) override;
	};

	struct Pole : CharacterContainer {
		void Handle(Character* c) override;
	};

	struct Scene : CharacterContainer {
		xx::Listi32<xx::Shared<SceneItem>> buildings;
		xx::Listi32<xx::Shared<Monster>> monsters;
		xx::Listi32<xx::Shared<Player>> players;
		int32_t Update() override;
		void Handle(Character* c) override;
		void Draw() override;
	};

	/************************************************************/

	inline int32_t Scene::Update() {
		for (auto i = buildings.len - 1; i >= 0; --i) {
			if (buildings[i]->Update()) {
				buildings.SwapRemoveAt(i);
			}
		}
		for (auto i = monsters.len - 1; i >= 0; --i) {
			if (monsters[i]->Update()) {
				monsters.SwapRemoveAt(i);
			}
		}
		for (auto i = players.len - 1; i >= 0; --i) {
			if (players[i]->Update()) {
				players.SwapRemoveAt(i);
			}
		}
		return 0;
	}

	inline void Scene::Draw() {
	}

	inline void Character::AttachTo(CharacterContainer* tar) {
		// assert(!tar->Contains(this));
		if (attachedTo) {
			// todo: remove from attachedTo->attachedCharacters
		}
		tar->attachedCharacters.Emplace(xx::WeakFromThis(this));
	}


	inline int32_t Character::Update() {
		attachedTo->Handle(this);
		return 0;
	}

	inline void Scene::Handle(Character* c) {
		// fly left or right
		//if (input.pressed("A or D")) c.pos.x += -= speed
	}

	inline void Block::Handle(Character* c) {
		// walk, jump, crouch ...
		//
	}

	inline void Pole::Handle(Character* c) {
		// grab? jump? walk?
		//
	}

}
