#include "broadcaster.h"

#include <algorithm>

#include <spdlog/spdlog.h>

#include "../common/States.h"

#include "Player.h"
#include "battlefield.h"

void BroadCaster::add_queue(const uint8_t& id, Queue<std::shared_ptr<States>>& state_queue) {
    std::lock_guard<std::mutex> l(m);
    broadcast_map.insert({id, &state_queue});
}

void BroadCaster::broadcast(const std::list<std::shared_ptr<States>>& game_states) {
    std::lock_guard<std::mutex> lock(m);
    for (auto& [id, client_game_state_queue]: broadcast_map) {
        try {
            for (const auto& gs: game_states) {
                client_game_state_queue->push(gs);
            }

        } catch (const ClosedQueue& e) {  // Se ignoran queues cerradas, luego se eliminan
            continue;
        }
    }
}

void BroadCaster::broadcastLobby() {
    std::list<std::shared_ptr<States>> states;
    infoParser.makeLobbyState(states);
    broadcast(states);
}

void BroadCaster::broadcastLevelLayout(const std::string& map_name) {
    std::list<std::shared_ptr<States>> states;
    infoParser.makeLevelState(states, map_name);
    broadcast(states);
}

void BroadCaster::broadcastGame(const uint8_t& id_of_active_player) {
    std::list<std::shared_ptr<States>> states;
    infoParser.makeGameState(states, id_of_active_player);
    broadcast(states);
}

void BroadCaster::remove_closed_clients(uint8_t& ready_count,
                                        std::map<uint8_t, std::unique_ptr<Player>>& players) {
    std::lock_guard<std::mutex> lock(m);

    auto it = broadcast_map.cbegin();
    while (it != broadcast_map.cend()) {
        if (it->second->is_closed()) {
            uint8_t id = it->first;
            it = broadcast_map.erase(it);

            if (players.count(id) != 1) {
                continue;
            }

            players.erase(id);

            ready_count--;
            continue;
        }

        ++it;
    }
}

void BroadCaster::removeLobbyPlayer(const uint8_t& player_id) {
    std::lock_guard<std::mutex> lock(m);
    broadcast_map.erase(player_id);
}
void BroadCaster::broadcast_turn(const uint8_t& player_turn, const bool& block_input) {
    std::lock_guard<std::mutex> lock(m);

    auto it = broadcast_map.begin();
    for (uint8_t i = 0; i < broadcast_map.size(); i++) {
        try {
            if (not block_input && i == player_turn) {
                it->second->push(std::make_shared<PlayerTurn>(IS_YOUR_TURN));
                ++it;
                continue;
            }

            it->second->push(std::make_shared<PlayerTurn>(NOT_YOUR_TURN));

            ++it;
        } catch (const ClosedQueue& e) {  // Se ignoran queues cerradas, luego se eliminan
            continue;
        }
    }
}

void BroadCaster::broadcastWin(std::map<uint8_t, std::unique_ptr<Player>>& players) {
    std::lock_guard<std::mutex> lock(m);

    auto it = broadcast_map.begin();
    auto player_it = players.begin();
    for (uint8_t i = 0; i < broadcast_map.size(); i++) {
        try {
            if (player_it->second->is_playing) {
                it->second->push(std::make_shared<YouWin>(YOU_WIN));
                ++it;
                ++player_it;
                continue;
            }

            it->second->push(std::make_shared<YouWin>(YOU_LOSE));

            ++it;
            ++player_it;
        } catch (const ClosedQueue& e) {  // Se ignoran queues cerradas, luego se eliminan
            continue;
        }
    }
}
