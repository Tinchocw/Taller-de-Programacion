#ifndef SERVER_PARSER_H
#define SERVER_PARSER_H

#include <atomic>
#include <memory>

#include "../common/States.h"
#include "../common/queue.h"

class PlayerAction;
class LobbyAction;
class GameBrowser;
enum class Actions;

namespace ServerSide {

class Protocol;

class Parser {
public:
    /*
        @param c: Accion a realizar
        @param protocol: Protocolo del cliente para recibir los demas atributos de la accion
        @param id: ID del cliente

        @brief Crea la accion del game a realizar segun el valor de c
    */
    static std::shared_ptr<PlayerAction> makePlayerAction(const Actions& c,
                                                          ServerSide::Protocol& protocol,
                                                          const uint8_t id);
    /*
        @param c: Accion a realizar
        @param protocol: Protocolo del cliente para recibir los demas atributos de la accion
        @param id: ID del cliente
        @param browser: Referencia al game browser del server
        @param game_id: ID del juego
        @param state_queue: Queue de states del jugador

        @brief Crea la accion del lobby a realizar segun el valor de c
    */
    static std::shared_ptr<LobbyAction> makeLobbyAction(
            const Actions& c, ServerSide::Protocol&, GameBrowser& browser, int16_t& game_id,
            const uint8_t& id, Queue<std::shared_ptr<States>>& state_queue);
};
}  // namespace ServerSide


#endif
