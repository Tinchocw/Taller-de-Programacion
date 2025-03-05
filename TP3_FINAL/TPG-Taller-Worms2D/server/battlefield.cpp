#include "battlefield.h"

#include <string>

#include "Player.h"
#include "crate.h"
#include "proyectile.h"
#include "worm_handler.h"

Battlefield::Battlefield(std::string level_selected):
        projectile_count(0), crate_count(0), level_holder(*this) {
    // Concat "_bars" to the level name
    level_selected += "_bars";
    for (const auto& element: Config::levelLayoutNode[level_selected]) {
        // Extract values from the YAML node
        auto x = element[0].as<float>();
        auto y = element[1].as<float>();
        auto angle = element[2].as<float>();
        auto is_long = element[3].as<bool>();

        level_holder.add_bar(x, y, (angle * M_PI), is_long);
    }
}

void Battlefield::updateProjectilesTimer() {
    for (auto& projectile: projectiles) {
        projectile.second->updateTimer();
    }
}

void Battlefield::post_action_explosion() {
    for (auto& projectile: projectiles) {
        projectile.second->second_collision_reaction();
    }
}

std::map<uint8_t, std::shared_ptr<Projectile>>& Battlefield::getProjectiles() {
    return projectiles;
}

void Battlefield::destroy_body(b2Body*& body) { engine.destroy_body(body); }

b2Body* Battlefield::add_body(b2BodyDef& bodyDef) { return engine.add_body(bodyDef); }

void Battlefield::add_query_AABB(b2QueryCallback* callback, const b2AABB& aabb) {
    engine.add_query_AABB(callback, aabb);
}

void Battlefield::step() {
    updateProjectilesTimer();
    engine.step();
    post_action_explosion();
    update_drown_projectiles();
    update_drown_crates();
}

void Battlefield::newWindForce(const bool& no_wind_cheat_activated) {
    engine.newWindForce(no_wind_cheat_activated);
}

void Battlefield::add_projectile(std::shared_ptr<Projectile>& proyectile) {
    projectiles.insert({projectile_count++, proyectile});
}

void Battlefield::remove_dead_objects() {
    auto it = projectiles.cbegin();
    while (it != projectiles.cend()) {
        if (it->second->is_dead()) {
            it = projectiles.erase(it);
            continue;
        }
        ++it;
    }

    crates.erase(std::remove_if(crates.begin(), crates.end(),
                                [](auto& crate) { return crate->wasOpened(); }),
                 crates.end());
}

const bool Battlefield::noProjectiles() { return projectiles.empty(); }

void Battlefield::createCrate() { crates.push_back(std::make_shared<Crate>(*this, crate_count++)); }

void Battlefield::update_drown_projectiles() {
    for (auto& projectile: projectiles) {
        projectile.second->drowning();
    }
}

void Battlefield::update_drown_crates() {
    for (auto& crate: crates) {
        crate->wasDrown();
    }
}
