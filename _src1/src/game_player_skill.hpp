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
            auto pos = player->pos;
            for (int i = 0; i < count; ++i) {
                // shoot nearest one
                auto p = player->pos;
                auto o = stage->monsters.FindNearestByRange(pos.x, pos.y, cfg->aimRange);
                if (o) {
                    auto dy = o->pos.y - pos.y;
                    auto dx = o->pos.x - pos.x;
                    auto r = std::atan2f(dy, dx);
                    auto cos = std::cosf(r);
                    auto sin = std::sinf(r);
                    auto d = ResTpFrames::_size_player1_.x - speedStep * i;
                    auto tarX = pos.x + cos * d;
                    auto tarY = pos.y + sin * d;
                    //new PlayerBullet(this).Init(tarX, tarY, r, cos, sin);
                    stage->playerBullets.Emplace().Emplace<PlayerBullet_1>()->Init(this);   // todo: args
                }
            }
        }
		return 0;
	}

}
