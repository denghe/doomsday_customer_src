#pragma once

namespace Game {

	// for shooter
	struct MonsterProperties1 {
		float healthMax{};
		float health{};
		// todo
	};

	// for bullet
	struct MonsterProperties2 {
		float damagePoint{};
		// todo
	};

	struct MonsterProperties : MonsterProperties1, MonsterProperties2 {
		void Init() {
			health = healthMax = 100.f;
			damagePoint = 10.f;
		}

		// return actual hurt dp, int: 0 normal 1 dodge 2 death
		XX_INLINE std::pair<float, int> Hurt(float targetDamagePoint) {
			if (health <= targetDamagePoint) {
				auto d = health;
				health = 0;
				return { d, 2 };
			}
			else {
				health -= targetDamagePoint;
				return{ targetDamagePoint, 0 };
			}
		}

	};

}
