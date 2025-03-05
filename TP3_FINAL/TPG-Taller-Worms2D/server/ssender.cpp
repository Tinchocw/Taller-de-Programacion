#include "ssender.h"

#include <spdlog/spdlog.h>

#include "../common/liberror.h"

#include "sprotocol.h"

ServerSide::Sender::Sender(ServerSide::Protocol& protocol,
                           Queue<std::shared_ptr<States>>& game_states):
        protocol(protocol), game_states(game_states) {}

void ServerSide::Sender::run() {
    do {

        try {
            this->protocol.sendStates(game_states.pop());

        } catch (const ClosedQueue& e) {
            if (_keep_running) {
                spdlog::get("server")->error("Se cerro la game_state_queue del sender: {:s}",
                                             e.what());
            }
            break;

        } catch (const LibError& e) {
            if (_keep_running) {
                spdlog::get("server")->error("Se cerro el socket del protocolo en sender: {:s}",
                                             e.what());
            }
            break;
        }

    } while (this->_keep_running);

    game_states.close();
}
