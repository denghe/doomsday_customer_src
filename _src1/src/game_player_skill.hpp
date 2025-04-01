#pragma once

namespace Game {

	inline void PlayerSkill_1::Init(Player* ownerPlayer_) {
		ownerPlayer = ownerPlayer_;
        auto& c = cfg.Emplace<PlayerSkillCfg_1>();
        c->aimRange = Cfg::unitSize * 10;
		c->radius = ResTpFrames::_size_bullet_coin5_.x * 0.5f;
		c->damage = 5;
		c->moveSpeed = 600.f / Cfg::fps;
        c->shootSpeed = 1.f / Cfg::fps;
		c->life = 3 * (int32_t)Cfg::fps;
		c->pierceCount = 0;
		c->pierceDelay = 0;
        c->knockbackForce = 123;
	}

	inline int32_t PlayerSkill_1::Update() {
        shootCountPool += cfg->shootSpeed;
        if (auto count = (int)shootCountPool; count > 0) {
            shootCountPool -= count;
            auto speedStep = cfg->moveSpeed / count;
            auto stage = ownerPlayer->ownerStage;
            auto pos = ownerPlayer->pos;
            for (int i = 0; i < count; ++i) {
                // shoot nearest one
                auto p = ownerPlayer->pos;
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
