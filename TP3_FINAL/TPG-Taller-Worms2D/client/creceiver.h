#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include <atomic>
#include <memory>

#include "../common/queue.h"
#include "../common/thread.h"

class States;

namespace ClientSide {
class Protocol;

class Receiver: public Thread {
private:
    ClientSide::Protocol& protocol;
    Queue<std::shared_ptr<States>>& game_stateQ;
    Queue<std::shared_ptr<States>>& lobby_stateQ;
    std::atomic<bool>& runned;

public:
    /*
        @brief Construye el receiver
    */
    explicit Receiver(ClientSide::Protocol& protocol,
                      Queue<std::shared_ptr<States>>& game_state_queue,
                      Queue<std::shared_ptr<States>>& lobby_state_queue, std::atomic<bool>& runned);
    /*
        @brief Corre el receiver recibiendo permanentemenete los states
    */
    void run() override;
    /*
        @brief Termina de forma forzosa el receiver
    */
    void kill();
    /*
        No queremos ni copiar ni mover el receiver
    */
    Receiver(const Receiver&) = delete;
    Receiver& operator=(const Receiver&) = delete;

    Receiver(Receiver&&) = delete;
    Receiver& operator=(Receiver&&) = delete;
};
}  // namespace ClientSide

#endif
