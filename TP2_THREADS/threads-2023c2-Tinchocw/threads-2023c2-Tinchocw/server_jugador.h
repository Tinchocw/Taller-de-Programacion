#ifndef THREADS_2023C2_TINCHOCW_SERVER_JUGADOR_H
#define THREADS_2023C2_TINCHOCW_SERVER_JUGADOR_H

#include <utility>

#include "common_queue.h"
#include "common_sockets.h"
#include "server_monitor_de_queues.h"
#include "server_protocol.h"
#include "server_receiver.h"
#include "server_sender.h"

namespace {  // NOLINT
const int SIZE_QUEUE = 30;
}

class Jugador {
private:
    Queue chats;
    Sender* sender;
    Receiver* receiver;
    Protocolo_servidor protocolo;
    Monitor_de_queues& lista_de_chats;

    void join();


public:
    explicit Jugador(Socket&& jugador, Monitor_de_queues& lobby);
    void start();
    bool estoy_conectado();
    void kill();

    // No permito la copia del jugador
    Jugador(const Jugador&) = delete;
    Jugador& operator=(const Jugador&) = delete;
    ~Jugador();
};


#endif  // THREADS_2023C2_TINCHOCW_SERVER_JUGADOR_H
