#include "crate_type.h"

#include <random>

#include "battlefield.h"
#include "entity.h"
#include "query_callback.h"


FirstAid::FirstAid() {
    auto rng = std::random_device();
    std::uniform_int_distribution<> random(MIN_HEALTH_FIRST_AID, MAX_HEALTH_FIRST_AID);

    health_quantity = random(rng);
}

void FirstAid::collision_reaction(b2Body*& crate_body, Battlefield& battlefield, bool& was_opened) {
    Query_callback queryCallback;
    b2AABB aabb;
    aabb.lowerBound = crate_body->GetPosition() - b2Vec2(CRATE_LENGTH / 2, CRATE_LENGTH / 2);
    aabb.upperBound = crate_body->GetPosition() + b2Vec2(CRATE_LENGTH / 2, CRATE_LENGTH / 2);
    battlefield.add_query_AABB(&queryCallback, aabb);

    // check which of these bodies have their center of mass within the blast radius
    for (int i = 0; i < queryCallback.found_bodies_size(); i++) {
        b2Body* body_ = queryCallback.found_bodie_at(i);

        auto entity = reinterpret_cast<Entity*>(body_->GetUserData().pointer);

        entity->recibe_life_modification(health_quantity);
        entity->open_crate(was_opened);
    }
}

AmmoBox::AmmoBox() {
    auto rng = std::random_device();
    std::uniform_int_distribution<> random(MIN_AMMO_AMMO_BOX, MAX_AMMO_AMMO_BOX);

    ammo_quantity = random(rng);
}

void AmmoBox::collision_reaction(b2Body*& crate_body, Battlefield& battlefield, bool& was_opened) {
    Query_callback queryCallback;
    b2AABB aabb;
    aabb.lowerBound = crate_body->GetPosition() - b2Vec2(CRATE_LENGTH / 2, CRATE_LENGTH / 2);
    aabb.upperBound = crate_body->GetPosition() + b2Vec2(CRATE_LENGTH / 2, CRATE_LENGTH / 2);
    battlefield.add_query_AABB(&queryCallback, aabb);

    // check which of these bodies have their center of mass within the blast radius
    for (int i = 0; i < queryCallback.found_bodies_size(); i++) {
        b2Body* body_ = queryCallback.found_bodie_at(i);

        auto entity = reinterpret_cast<Entity*>(body_->GetUserData().pointer);

        entity->reloadAmmo(ammo_quantity);
        entity->open_crate(was_opened);
    }
}

Trap::Trap() {
    auto rng = std::random_device();
    std::uniform_int_distribution<> random(MIN_DAMAGE_TRAP, MAX_DAMAGE_TRAP);

    epicenter_dmg = random(rng);
}

void Trap::collision_reaction(b2Body*& crate_body, Battlefield& battlefield, bool& was_opened) {
    Query_callback queryCallback;
    b2AABB aabb;
    aabb.lowerBound = crate_body->GetPosition() - b2Vec2(CRATE_LENGTH * 2, CRATE_LENGTH * 2);
    aabb.upperBound = crate_body->GetPosition() + b2Vec2(CRATE_LENGTH * 2, CRATE_LENGTH * 2);
    battlefield.add_query_AABB(&queryCallback, aabb);

    // check which of these bodies have their center of mass within the blast radius
    for (int i = 0; i < queryCallback.found_bodies_size(); i++) {
        b2Body* body_ = queryCallback.found_bodie_at(i);
        b2Vec2 bodyCom = body_->GetWorldCenter();

        b2Vec2 blastDir = bodyCom - crate_body->GetWorldCenter();

        if (blastDir.Length() >= CRATE_LENGTH * 2)
            continue;

        auto distance = (blastDir).Normalize();

        float damage = epicenter_dmg * ((CRATE_LENGTH * 2) - distance) / (CRATE_LENGTH * 2);
        auto entity = reinterpret_cast<Entity*>(body_->GetUserData().pointer);
        entity->apply_explosion(damage * blastDir);
        entity->recibe_life_modification(-damage);
        entity->open_crate(was_opened);
    }
}
