#include "csender.h"

#include <spdlog/spdlog.h>

#include "../common/liberror.h"

#include "Action.h"
#include "cprotocol.h"

ClientSide::Sender::Sender(ClientSide::Protocol& protocol,
                           Queue<std::shared_ptr<Action>>& action_queue):
        Thread(), protocol(protocol), action_queue(action_queue) {}

void ClientSide::Sender::run() {
    do {

        try {
            action_queue.pop()->send(protocol);

        } catch (const ClosedQueue& e) {
            if (_keep_running) {
                spdlog::get("client")->error("Se cerro la action_queue del sender: {:s}", e.what());
            }
            break;

        } catch (const LibError& e) {
            if (_keep_running) {
                spdlog::get("client")->error("Se cerro el socket del protocolo en sender: {:s}",
                                             e.what());
            }
            break;
        }

    } while (this->_keep_running);
}

void ClientSide::Sender::kill() {
    this->stop();
    this->action_queue.close();
}