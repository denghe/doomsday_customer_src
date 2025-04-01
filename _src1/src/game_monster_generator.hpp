#pragma once

namespace Game {

    void MonsterGenerator_1::Update() {
        auto& p = stage->player->pos;
        auto& rnd = stage->rnd;
        static constexpr auto ds_2 = Cfg::designSize / 2;
        for (countPool += countIncPerFrame; countPool >= 1; --countPool) {
            if (stage->monsters.items.len < Cfg::numMaxMonsters) {
                XY pos{};
                switch (stage->rnd.Next<int32_t>(0, 4)) {
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
                m->Init(stage, pos);
                stage->monsters.Add(std::move(m));
            }
        }
    }

}
