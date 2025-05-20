#pragma once

namespace Game {

    // T::Monster
    template<typename T>
    struct MonsterGen_Generic : MonsterGen {
        int32_t Update() override;
    };



    // requires T : Monster
    template<typename T>
    struct MonsterGen_Group : MonsterGen {

        int32_t numGroups{};
        int32_t groupDelayNumFrames{};

        int32_t numMonsters{};
        int32_t delayNumFrames{};
        float radius{};
        XY pos{};
        int32_t n{}, i{}, j{}, t{};

        void Init(Stage* stage_, float activeSeconds_, float destroySeconds_, int32_t numMonsters_, float radius_, float durationSeconds_, int32_t numGroups_);
        int32_t Update() override;
    };


    // requires T1 & T2 : Monster
    template<typename T1, typename T2>
    struct MonsterGen_Group2 : MonsterGen {

        int32_t numGroups{};
        int32_t groupDelayNumFrames{};

        int32_t numMonsters1{}, numMonsters2{};
        int32_t delayNumFrames{};
        float radius{};
        XY pos{};
        int32_t n{}, i{}, j{}, t{};

        void Init(Stage* stage_, float activeSeconds_, float destroySeconds_, int32_t numMonsters1_, int32_t numMonsters2_, float radius_, float durationSeconds_, int32_t numGroups_);
        int32_t Update() override;
    };


    // requires T1 & T2 & T3 : Monster
    template<typename T1, typename T2, typename T3>
    struct MonsterGen_Group3 : MonsterGen {

        int32_t numGroups{};
        int32_t groupDelayNumFrames{};

        int32_t numMonsters1{}, numMonsters2{}, numMonsters3{};
        int32_t delayNumFrames{};
        float radius{};
        XY pos{};
        int32_t n{}, i{}, j{}, t{};

        void Init(Stage* stage_, float activeSeconds_, float destroySeconds_, int32_t numMonsters1_, int32_t numMonsters2_, int32_t numMonsters3_, float radius_, float durationSeconds_, int32_t numGroups_);
        int32_t Update() override;
    };

}
