    #ifndef SERVER_RECEIVER_H
#define SERVER_RECEIVER_H

#include <memory>

#include <stdint.h>

#include "../common/States.h"
#include "../common/queue.h"
#include "../common/thread.h"

class GameBrowser;

namespace ServerSide {
class Protocol;

class Receiver: public Thread {
private:
    ServerSide::Protocol& protocol;
    GameBrowser& browser;
    int16_t room_id;
    Queue<std::shared_ptr<States>>& state_queue;

public:
    const uint8_t& id;
    /*
        @param protocol: Referencia al protocolo del cliente
        @param browser: Referencia al game browser
        @param state_queue: Referencia a la state queue del cliente
        @param id: Referencia a la id del jugador
    */
    explicit Receiver(ServerSide::Protocol& protocol, GameBrowser& browser,
                      Queue<std::shared_ptr<States>>& state_queue, const uint8_t& id);
    /*
        @brief Corre el receiver esperando a recibir datos del socket segun su estado en juego o en
       lobby
    */
    void run() override;
    /*
        Destructor default ya que no tiene que liberar ningun recurso
    */
    ~Receiver() override = default;
    /*
        No queremos ni mover ni copiar el Receiver
    */
    Receiver(const Receiver&) = delete;
    Receiver& operator=(const Receiver&) = delete;

    Receiver(Receiver&&) = delete;
    Receiver& operator=(Receiver&&) = delete;
};
}  // namespace ServerSide

#endif
