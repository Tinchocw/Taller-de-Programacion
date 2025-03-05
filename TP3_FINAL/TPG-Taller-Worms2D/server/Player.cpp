#include "Player.h"

#include <numeric>
#include <utility>

#include "../common/States.h"

#include "battlefield.h"
#include "engine.h"

Player::Player():
        ready(false),
        is_playing(true),
        selected_weapon(nullptr),
        selected_gadget_type(WeaponsAndTools::BAZOOKA),
        worm_turn(0),
        allow_multiple_jump(false),
        immortal_worms(false) {
    weapons.insert({WeaponsAndTools::BAZOOKA, std::make_unique<Bazooka>()});
    weapons.insert({WeaponsAndTools::MORTAR, std::make_unique<Mortar>()});
    weapons.insert({WeaponsAndTools::GREEN_GRENADE, std::make_unique<GreenGrenade>()});
    weapons.insert({WeaponsAndTools::RED_GRENADE, std::make_unique<RedGrenade>()});
    weapons.insert({WeaponsAndTools::BANANA, std::make_unique<BananaGrenade>()});
    weapons.insert({WeaponsAndTools::HOLY_GRENADE, std::make_unique<HolyGrenade>()});
    weapons.insert({WeaponsAndTools::DYNAMITE, std::make_unique<DynamiteGrenade>()});
    weapons.insert({WeaponsAndTools::BASEBALL_BAT, std::make_unique<BaseballBat>()});
    weapons.insert({WeaponsAndTools::AIR_STRIKE, std::make_unique<AirStrike>()});
    weapons.insert({WeaponsAndTools::TELEPORT, std::make_unique<Teleport>()});

    selected_weapon = &weapons.at(WeaponsAndTools::BAZOOKA);
}

uint8_t Player::calcAvgLife() {
    float life_sum = std::accumulate(
            worms.begin(), worms.end(), 0.0f,
            [](const float& sum, const auto& worm) { return sum + worm.second->life; });

    return (uint8_t)(life_sum / worms.size());
}

void Player::set_ready() { ready = !ready; }

void Player::spawnWorms(Battlefield& battlefield, const uint8_t& worms_quantity,
                        uint8_t& worm_counter, const uint8_t& id,
                        std::vector<b2Vec2>& spawn_points) {


    for (uint8_t i = 0; i < worms_quantity; i++) {
        worms.insert(
                {worm_counter++, std::make_shared<Worm>(battlefield, selected_weapon,
                                                        selected_gadget_type, allow_multiple_jump,
                                                        immortal_worms, spawn_points.back(), id)});
        spawn_points.pop_back();
    }
}

std::unique_ptr<AmmoLeft> Player::getWeaponsAmmo() {
    auto ammo_left = std::make_unique<AmmoLeft>();
    auto it = weapons.cbegin();
    while (it != weapons.cend()) {
        ammo_left->weapon_ammo.insert({it->first, it->second->ammo});
        ++it;
    }
    return ammo_left;
}

void Player::change_weapon(const WeaponsAndTools& new_weapon) {
    if (weapons.count(new_weapon) != 1) {
        return;
    }
    selected_weapon = &weapons.at(new_weapon);
    selected_gadget_type = new_weapon;
}

Player::Player(Player&& o):
        ready(o.ready),
        is_playing(o.is_playing),
        selected_weapon(o.selected_weapon),
        selected_gadget_type(o.selected_gadget_type),
        worm_turn(o.worm_turn),
        allow_multiple_jump(o.allow_multiple_jump),
        immortal_worms(o.immortal_worms) {
    o.selected_weapon = nullptr;
    o.ready = false;
    o.is_playing = false;
    o.worm_turn = 0;
    o.allow_multiple_jump = false;
    o.immortal_worms = false;
}

void Player::infiniteAmmo() {
    for (const auto& weapon: weapons) {
        weapon.second->infiniteAmmo();
    }
}
