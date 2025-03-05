#include "info_parser.h"

#include <algorithm>
#include <utility>

#include "../common/States.h"

#include "Game.h"
#include "Player.h"
#include "crate.h"
#include "proyectile.h"

void InfoParser::makeLobbyState(std::list<std::shared_ptr<States>>& states) {
    /*
        Sin lock, ya que previamente game coloca el lock
    */
    states.push_back(std::make_shared<PlayerCountL>(game.players.size()));

    std::transform(game.players.begin(), game.players.end(), std::back_inserter(states),
                   [](const auto& player) {
                       return std::make_shared<PlayerStateL>(player.second->ready, player.first);
                   });
}

void InfoParser::makeLevelState(std::list<std::shared_ptr<States>>& states, const std::string& map_name) {
    states.push_back((game.battlefield.level_holder.get_level_building_state(map_name)));
}

void InfoParser::makeGameState(std::list<std::shared_ptr<States>>& states,
                               const uint8_t& id_of_active_player) {
    // WIND
    states.push_back(
            std::make_shared<BattlefieldState>(game.battlefield.engine.wind_force));

    // CRATES

    states.push_back(std::make_shared<CrateCount>(game.battlefield.crates.size()));

    std::transform(game.battlefield.crates.begin(), game.battlefield.crates.end(),
                   std::back_inserter(states), [](const auto& crate) {
                       return std::make_shared<CrateState>(
                               crate->body->GetPosition().x, crate->body->GetPosition().y,
                               crate->falling, crate->was_opened, crate->_type, crate->crate_id);
                   });

    // PROJECTILES
    states.push_back(std::make_shared<ProjectileCount>(game.battlefield.projectiles.size()));

    std::transform(game.battlefield.projectiles.begin(), game.battlefield.projectiles.end(),
                   std::back_inserter(states), [](const auto& projectile) {
                       return projectile.second->get_projectile_state(projectile.first);
                   });

    // WORM & PLAYERS
    auto it = game.players.at(id_of_active_player)->worms.begin();
    advance(it, game.players.at(id_of_active_player)->worm_turn);

    uint8_t id_of_active_worm = (*it).first;

    for (const auto& [id, player]: game.players) {
        states.push_back(std::make_shared<PlayerStateG>(
                player->is_playing, id == id_of_active_player,
                id, player->calcAvgLife(), player->getWeaponsAmmo()));

        std::transform(player->worms.begin(), player->worms.end(), std::back_inserter(states),
                       [&id_of_active_worm](const auto& worm) {
                           return std::make_shared<WormStateG>(
                                   worm.first, worm.second->body->GetPosition().x,
                                   worm.second->body->GetPosition().y, worm.second->weapon_type,
                                   ((worm.first == id_of_active_worm)), worm.second->is_walking,
                                   worm.second->is_jumping, worm.second->is_backflipping,
                                   worm.second->facing_right, worm.second->falling,
                                   worm.second->aim_inclination_degrees,
                                   worm.second->charging_shoot, worm.second->life,
                                   worm.second->drown, worm.second->using_tool, worm.second->team);
                       });
    }
}
