#include "crate.h"

#include <random>

#include "battlefield.h"

Crate::Crate(Battlefield& battlefield, const uint8_t& id):
        Entity(battlefield),
        type(nullptr),
        _type(_CrateType_::TRAP),
        falling(true),
        was_opened(false),
        crate_id(id) {
    auto rng = std::random_device();
    std::mt19937 gen(rng());
    std::uniform_int_distribution<> random(0, 2);

    _type = (_CrateType_)random(gen);

    switch (_type) {
        case _CrateType_::FIRST_AID:
            type = std::make_shared<FirstAid>();
            break;

        case _CrateType_::AMMO_BOX:
            type = std::make_shared<AmmoBox>();
            break;

        case _CrateType_::TRAP:
            type = std::make_shared<Trap>();
            break;
    }

    std::uniform_real_distribution<float> crate_pos(MIN_X_CRATE_SPAWN, MAX_X_CRATE_SPAWN);

    b2BodyDef crateDef;
    crateDef.type = b2_dynamicBody;

    auto crate_x = crate_pos(gen);
    crateDef.position.Set(crate_x, Y_CRATE_SPAWN);
    crateDef.allowSleep = true;
    crateDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

    body = battlefield.add_body(crateDef);
    b2PolygonShape crateBox;
    crateBox.SetAsBox(CRATE_LENGTH / 2, CRATE_LENGTH / 2);

    b2FixtureDef fixtureDef;
    fixtureDef.filter.groupIndex = -3;
    fixtureDef.shape = &crateBox;
    fixtureDef.density = 4.0f;
    fixtureDef.friction = 0.8;

    body->SetGravityScale(0.15);
    body->CreateFixture(&fixtureDef);
}

void Crate::collision_reaction(const b2Vec2& normal) {
    if (falling) {
        return;
    }

    if (was_opened) {
        return;
    }

    type->collision_reaction(body, battlefield, was_opened);

    if (was_opened) {
        dead = true;
    }
}

void Crate::stop_falling() {
    falling = false;
    body->SetAwake(false);
}

const bool Crate::wasOpened() { return was_opened; }

const bool Crate::wasDrown() {
    if (body->GetPosition().y < 0) {
        was_opened = true;
        falling = false;
    }
    return was_opened;
}

Crate::~Crate() {}
