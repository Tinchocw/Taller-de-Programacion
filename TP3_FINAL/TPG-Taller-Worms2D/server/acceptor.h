#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <atomic>
#include <map>
#include <memory>

#include "../common/socket.h"
#include "../common/thread.h"

#include "game_browser.h"

namespace ServerSide {
class Protocol;
}

#define SHUTDOWN_RW 2

class Acceptor: public Thread {

private:
    Socket skt;
    GameBrowser gb;
    std::map<uint8_t, std::unique_ptr<ServerSide::Client>> clients_connected;

    void reap_dead();

public:
    explicit Acceptor(const char* servname);

    /*
        @brief Acepta clientes
     */
    void run() override;

    /*
        @brief Realiza un shutdown y close del socket
     */
    void kill();

    ~Acceptor();

    /*
        @brief No queremos ni copiar ni mover el acceptor
     */
    Acceptor(const Acceptor&) = delete;
    Acceptor& operator=(const Acceptor&) = delete;

    Acceptor(Acceptor&&) = delete;
    Acceptor& operator=(Acceptor&&) = delete;
};

#endif
