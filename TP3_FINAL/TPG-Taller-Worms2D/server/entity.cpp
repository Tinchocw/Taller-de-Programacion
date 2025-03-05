#include "entity.h"

#include "battlefield.h"

Entity::Entity(Battlefield& battlefield): dead(false), body(nullptr), battlefield(battlefield) {}

void Entity::stop_falling() {}

void Entity::start_falling() {}

void Entity::recibe_life_modification(const float& life_variation) {}

bool Entity::is_dead() { return dead; }

void Entity::apply_explosion(const b2Vec2& final_impulse) {
    body->ApplyLinearImpulseToCenter(final_impulse, true);
}

Entity::~Entity() { battlefield.destroy_body(body); }
