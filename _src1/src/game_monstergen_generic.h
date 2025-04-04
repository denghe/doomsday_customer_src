#pragma once

namespace Game {

    // T::Monster
    template<typename T>
    struct MonsterGen_Generic : MonsterGen {
        int32_t Update() override;
    };
}
