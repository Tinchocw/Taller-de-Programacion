#ifndef SERVER_SENDER_H
#define SERVER_SENDER_H

#include <memory>

#include "../common/States.h"
#include "../common/queue.h"
#include "../common/thread.h"

class GameLoop;

namespace ServerSide {
class Protocol;

class Sender: public Thread {
private:
    ServerSide::Protocol& protocol;
    Queue<std::shared_ptr<States>>& game_states;

public:
    /*
        @param protocol: Referencia al protocolo del cliente
        @param game_states: Referencia a la queue del cliente
    */
    explicit Sender(ServerSide::Protocol& protocol, Queue<std::shared_ptr<States>>& game_states);
    /*
        @brief Corre el sender esperando que la event_queue tenga un elemento para poder enviar
        a traves del protocolo
    */
    void run() override;
    /*
        @brief No queremos ni mover ni copiar el sender
    */
    Sender(const Sender&) = delete;
    Sender& operator=(const Sender&) = delete;

    Sender(Sender&&) = delete;
    Sender& operator=(Sender&&) = delete;
};
}  // namespace ServerSide

#endif
