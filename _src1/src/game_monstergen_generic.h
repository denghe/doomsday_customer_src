#pragma once

namespace Game {

    // T::Monster
    template<typename T>
    struct MonsterGen_Generic : MonsterGen {
        int32_t Update() override;
    };


    struct MonsterGen_2xHamburger : MonsterGen {
        static constexpr float cRadius{ 2 };
        static constexpr float cDurationSeconds{ 1 };
        static constexpr int32_t cDurationNumFrames{ int32_t(Cfg::fps * cDurationSeconds) };
        static constexpr int32_t cNumMonsters{ 2 };
        static constexpr int32_t cStepNumFrames{ cDurationNumFrames / cNumMonsters };

        int32_t numGroups{};
        int32_t groupDelayNumFrames{};
        int32_t n{}, i{}, j{}, t{};
        XY pos{};

        void Init(Stage* stage_, float activeSeconds_, float destroySeconds_, int32_t numGroups_);
        int32_t Update() override;
    };

}
