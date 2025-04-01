#pragma once

namespace Game {

	inline void Skill_1::Init(Player* player_, SkillCfg* skillCfg_) {
		player = player_;
		cfg = skillCfg_;
	}

	inline int32_t Skill_1::Update() {
        shootCountPool += cfg->shootSpeed;
        if (auto count = (int)shootCountPool; count > 0) {
            shootCountPool -= count;
            auto speedStep = cfg->moveSpeed / count;
            auto stage = player->stage;
            auto pp = player->pos;
            for (int i = 0; i < count; ++i) {
                // shoot nearest one
                auto o = stage->monsters.FindNearestByRange(pp.x, pp.y, cfg->aimRange);
                if (o) {
                    auto dy = o->pos.y - pp.y;
                    auto dx = o->pos.x - pp.x;
                    auto r = std::atan2f(dy, dx);
                    auto cos = std::cosf(r);
                    auto sin = std::sinf(r);
                    auto d = ResTpFrames::_size_player1_.x - speedStep * i;
                    auto tarX = pp.x + cos * d;
                    auto tarY = pp.y + sin * d;
                    stage->playerBullets.Emplace().Emplace<PlayerBullet_1>()->Init(this
                        , pp + XY{ cos * d, sin * d }, 0, 1, cos, sin);
                }
            }
        }
		return 0;
	}

}
