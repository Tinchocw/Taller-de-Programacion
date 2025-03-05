#include "cparser.h"

#include <chrono>
#include <thread>

#include "../common/States.h"

void ClientSide::Parser::filterStates(Queue<std::shared_ptr<States>>& game_states,
                                      Queue<std::shared_ptr<States>>& lobby_states,
                                      const std::shared_ptr<States>& state) {
    switch (state->tag) {
        case StatesTag::GAMES_COUNT_L:
        case StatesTag::GAME_NOT_JOINABLE:
        case StatesTag::INFO_GAME_L:
        case StatesTag::PLAYER_COUNT_L:
        case StatesTag::PLAYER_L:
            lobby_states.push(state);
            break;
        default:
            game_states.push(state);
            break;
    }
}
