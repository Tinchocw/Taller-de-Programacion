#include "engine.h"

#include <algorithm>

Engine::Engine():
        gravity(X_GRAVITY, Y_GRAVITY), world(std::make_unique<b2World>(gravity)), wind_force(0) {
    listener = new Contact_listener();
    create_battlefield();
    rng = std::mt19937(std::random_device{}());
    random_distribution = std::uniform_real_distribution<float>(MIN_WIND, MAX_WIND);
}

void Engine::create_battlefield() { world->SetContactListener(listener); }

b2Body* Engine::add_body(b2BodyDef& bodyDef) { return world->CreateBody(&bodyDef); }

void Engine::step() {
    applyWindForce();
    world->Step(TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
}

void Engine::destroy_body(b2Body* body) { world->DestroyBody(body); }

void Engine::add_query_AABB(b2QueryCallback* callback, const b2AABB& aabb) {
    world->QueryAABB(callback, aabb);
}

void Engine::newWindForce(const bool& no_wind_cheat_activated) {
    this->wind_force = random_distribution(rng) * (!no_wind_cheat_activated);
}

void Engine::applyWindForce() {
    auto body_list = world->GetBodyList();

    for (int32 i = 0; i < world->GetBodyCount(); ++i) {
        auto entity = reinterpret_cast<Entity*>(body_list->GetUserData().pointer);

        if (not entity) {
            continue;
        }

        entity->applyWindResistance(wind_force);

        if (not body_list->GetNext()) {
            break;
        }

        body_list = body_list->GetNext();
    }
}


Engine::~Engine() { delete listener; }
