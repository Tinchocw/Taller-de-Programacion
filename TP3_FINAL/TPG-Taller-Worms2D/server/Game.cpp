#include "Game.h"

#include <algorithm>
#include <numeric>
#include <utility>

#include <spdlog/spdlog.h>

#include "game_loop.h"
#include "proyectile.h"


bool Game::non_locking_is_playing() {
    auto sz = players.size();
    return (sz > 1 && sz == ready_count);
}

void Game::notify_lobby_state() { broadcaster.broadcastLobby(); }

Queue<std::shared_ptr<PlayerAction>>& Game::get_action_queue() {
    return this->gameloop.get_action_queue();
}

void Game::add_client_queue(const uint8_t& id, Queue<std::shared_ptr<States>>& state_queue) {
    std::lock_guard<std::mutex> lock(m);

    if (non_locking_is_playing()) {
        state_queue.push(std::make_shared<GameNotJoinable>());
        return;
    }

    if (players.size() >= MAX_PLAYERS) {
        state_queue.push(std::make_shared<GameNotJoinable>());
        return;
    }

    broadcaster.add_queue(id, state_queue);
    players.insert(std::make_pair(id, std::make_unique<Player>()));
    notify_lobby_state();
}

void Game::removeLobbyPlayer(const uint8_t& player_id) {
    std::lock_guard<std::mutex> lock(m);

    if (players.count(player_id) != 1) {
        return;
    }

    if (players.at(player_id)->ready) {
        ready_count--;
    }
    players.erase(player_id);
    broadcaster.removeLobbyPlayer(player_id);

    notify_lobby_state();
}

bool Game::isEmpty() {
    std::lock_guard<std::mutex> lock(m);
    return players.empty();
}

bool Game::is_playing() {
    std::lock_guard<std::mutex> lock(m);
    auto sz = players.size();
    return (sz > 1 && sz == ready_count);
}

void Game::set_player_ready(const uint8_t& id) {
    std::lock_guard<std::mutex> lock(m);
    // Esta unido al game el player[id]?
    if (players.count(id) == 1) {
        players.at(id)->set_ready();
        // Resta si el player dio unready, inc si dio ready
        ready_count += std::pow(-1, 1 - players.at(id)->ready);
    }

    notify_lobby_state();

    // Inicio el gl si estan todos listos y no esta iniciado ya
    if (not gameloop.is_alive() && this->non_locking_is_playing()) {
        spawnWorms();
        broadcaster.broadcastLevelLayout(map_name);
        gameloop.start();
        need_to_join_loop = true;
    }
}

void Game::spawnWorms() {
    size_t min_worms_player = WORMS_QUANTITY / players.size();
    size_t no_assigned_worms = WORMS_QUANTITY % players.size();

    uint8_t team = 0;

    for (auto& [id, player]: players) {
        player->spawnWorms(battlefield, min_worms_player, worm_counter, team++, spawn_points);
    }

    // Terminar si no hay worms que falten asignar
    if (not no_assigned_worms) {
        return;
    }

    team = 0;

    auto it = players.cbegin();
    for (size_t i = 0; i < no_assigned_worms; i++) {
        it->second->spawnWorms(battlefield, 1, worm_counter, team++, spawn_points);
        ++it;
    }

    while (it != players.cend()) {
        for (auto& worm: it->second->worms) {
            worm.second->recibe_life_modification(EXTRA_LIFE);
        }

        ++it;
    }
}

std::shared_ptr<GameInfoL> Game::getInfo() {
    std::lock_guard<std::mutex> lock(m);
    return std::make_shared<GameInfoL>(description, map_name, players.size(), game_id);
}
Game::~Game() {
    spdlog::get("server")->debug("Joineando gameloop");
    if (need_to_join_loop) {
        gameloop.join();
    }
}
