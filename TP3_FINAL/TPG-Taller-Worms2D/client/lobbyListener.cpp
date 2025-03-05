#include "lobbyListener.h"

#include <thread>

#include "../common/const.h"

#include "game_frame.h"
#include "player_frame.h"

void LobbyListener::setGameFrames(std::vector<std::unique_ptr<GameFrame>>& games,
                                  Queue<std::shared_ptr<States>>& lobby_states,
                                  const uint8_t& games_q) {
    std::shared_ptr<States> raw_state = nullptr;
    std::shared_ptr<GameInfoL> game_info = nullptr;
    for (uint8_t i = 0; i < games_q; i++) {
        do {
            while (not lobby_states.try_pop(raw_state)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 30));
            }
        } while (raw_state->tag != StatesTag::INFO_GAME_L);
        game_info = std::dynamic_pointer_cast<GameInfoL>(raw_state);
        games[i]->setFrame(game_info->description, game_info->map, game_info->player_count,
                           game_info->game_id);
        games[i]->show();
    }
}

uint8_t LobbyListener::getGameFramesQuantity(Queue<std::shared_ptr<States>>& lobby_states) {
    std::shared_ptr<States> raw_state = nullptr;

    do {
        while (not lobby_states.try_pop(raw_state)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 30));
        }

        if (raw_state->tag == StatesTag::CONNECTION_ERROR_STATE) {
            return CONNECTION_ERROR;
        }

    } while (raw_state->tag != StatesTag::GAMES_COUNT_L);

    return std::dynamic_pointer_cast<GamesCountL>(raw_state)->quantity;
}

uint8_t LobbyListener::getPlayersInLobbyQuantity(Queue<std::shared_ptr<States>>& lobby_states) {
    std::shared_ptr<States> raw_state = nullptr;

    if (not lobby_states.try_pop(raw_state)) {
        return NOT_POPPED_COUNT;
    }

    if (raw_state->tag == StatesTag::CONNECTION_ERROR_STATE) {
        return CONNECTION_ERROR;
    }

    if (raw_state->tag == StatesTag::GAME_NOT_JOINABLE) {
        return NOT_JOINABLE;
    }

    if (raw_state->tag != StatesTag::PLAYER_COUNT_L) {
        return NOT_POPPED_COUNT;
    }

    return std::dynamic_pointer_cast<CountState>(raw_state)->quantity;
}

void LobbyListener::setPlayers(std::map<uint8_t, std::unique_ptr<PlayerFrame>>& players,
                               Queue<std::shared_ptr<States>>& lobby_states,
                               const uint8_t& players_q) {
    std::shared_ptr<States> raw_state = nullptr;
    std::shared_ptr<PlayerStateL> player_info = nullptr;
    for (uint8_t i = 0; i < players_q; i++) {
        while (not lobby_states.try_pop(raw_state)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 30));
        }

        if (raw_state->tag == StatesTag::PLAYER_L) {
            player_info = std::dynamic_pointer_cast<PlayerStateL>(raw_state);
            players.insert({player_info->id,
                            std::make_unique<PlayerFrame>(player_info->id, player_info->ready)});
        }
    }
}
