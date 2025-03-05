#ifndef SERVER_H
#define SERVER_H

#include <memory>

#include "acceptor.h"
#include "sprotocol.h"
#include "sreceiver.h"
#include "ssender.h"

class Server {

private:
    Acceptor acceptor;

public:
    /*
        @param servname: Puerto en el cual inicializar el protocolo

        @brief Crea el servidor junto a su protocolo
    */
    explicit Server(char* servname);
    /*
        @brief Corre el servidor
    */
    void run();
    /*
        @brief Libera la memoria alocada de acceptor
    */
    ~Server();
    /*
        No queremos ni copiar ni mover el servidor
    */
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    Server(Server&&) = delete;
    Server& operator=(Server&&) = delete;
};


#endif
