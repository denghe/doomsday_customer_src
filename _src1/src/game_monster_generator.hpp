#pragma once

namespace Game {

    void MonsterGenerator_1::Update() {
        auto& p = ownerStage->player->pos;
        auto& rnd = ownerStage->rnd;
        static constexpr auto ds_2 = Cfg::designSize / 2;
        for (countPool += countIncPerFrame; countPool >= 1; --countPool) {
            if (ownerStage->monsters.items.len < Cfg::numMaxMonsters) {
                XY pos{};
                switch (ownerStage->rnd.Next<int32_t>(0, 4)) {
                case 0:
                    pos = { p.x - ds_2.x - Cfg::unitSize
                        , p.y + rnd.Next<float>(-ds_2.y - Cfg::unitSize, ds_2.y + Cfg::unitSize) };
                    break;
                case 1:
                    pos = { p.x + ds_2.x + Cfg::unitSize
                        , p.y + rnd.Next<float>(-ds_2.y - Cfg::unitSize, ds_2.y + Cfg::unitSize) };
                    break;
                case 2:
                    pos = { p.x + rnd.Next<float>(-ds_2.x - Cfg::unitSize, ds_2.x + Cfg::unitSize)
                        , p.y - ds_2.y - Cfg::unitSize };
                    break;
                case 3:
                    pos = { p.x + rnd.Next<float>(-ds_2.x - Cfg::unitSize, ds_2.x + Cfg::unitSize)
                        , p.y + ds_2.y + Cfg::unitSize };
                    break;
                }
                auto m = xx::MakeShared<Monster_1>();
                m->Init(ownerStage, pos);
                ownerStage->monsters.Add(std::move(m));
            }
        }
    }

}
