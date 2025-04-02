#pragma once

namespace Game {

    struct MonsterGenerator_1 : MonsterGen {
        void Update() override;
    };

    struct MonsterGenerator_Chips : MonsterGen {
        void Update() override;
    };

    struct MonsterGenerator_Hamburger : MonsterGen {
        void Update() override;
    };

    struct MonsterGenerator_Cola : MonsterGen {
        void Update() override;
    };

    struct MonsterGenerator_Instantnoodles : MonsterGen {
        void Update() override;
    };
}
