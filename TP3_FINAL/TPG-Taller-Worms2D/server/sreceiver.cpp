#include "sreceiver.h"

#include <spdlog/spdlog.h>

#include "../common/liberror.h"

#include "game_browser.h"
#include "game_loop.h"
#include "player_action.h"
#include "sparser.h"
#include "sprotocol.h"

ServerSide::Receiver::Receiver(ServerSide::Protocol& protocol, GameBrowser& gb,
                               Queue<std::shared_ptr<States>>& state_queue, const uint8_t& id):
        protocol(protocol), browser(gb), room_id(-1), state_queue(state_queue), id(id) {}

void ServerSide::Receiver::run() {
    Actions c;
    do {
        try {
            while (not browser.game_started_playing(room_id)) {
                protocol.recvCommand(c);
                ServerSide::Parser::makeLobbyAction(c, protocol, browser, room_id, id, state_queue)
                        ->execute();
            }

            Queue<std::shared_ptr<PlayerAction>>& q = browser.getQueue(room_id);

            while (browser.game_started_playing(room_id)) {
                protocol.recvCommand(c);
                q.push(ServerSide::Parser::makePlayerAction(c, protocol, id));
            }

        } catch (const ClosedQueue& e) {
            if (_keep_running) {
                spdlog::get("server")->error("Se cerro la action_queue del receiver: {:s}",
                                             e.what());
            }
            break;

        } catch (const LibError& e) {
            if (_keep_running) {
                spdlog::get("server")->error("Se cerro el socket del protocolo en receiver: {:s}",
                                             e.what());
            }
            break;
        }

    } while (_keep_running);
}
