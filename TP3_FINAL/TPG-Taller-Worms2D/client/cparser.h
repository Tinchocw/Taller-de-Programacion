#ifndef CLIENT_PARSER_H
#define CLIENT_PARSER_H

#include <map>
#include <memory>
#include <vector>

#include "../common/const.h"
#include "../common/queue.h"

class States;

namespace ClientSide {

class Parser {

public:
    /*
        @param game_states: Queue de game states del eventloop
        @param lobby_states: Queue de lobby states del eventloop
        @param state: State a ser filtrado

        @brief Filtra el state segun si pertenece a un state de lobby o de game
    */
    static void filterStates(Queue<std::shared_ptr<States>>& game_states,
                             Queue<std::shared_ptr<States>>& lobby_states,
                             const std::shared_ptr<States>& state);
};
}  // namespace ClientSide

#endif
