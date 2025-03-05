#ifndef THREADS_2023C2_TINCHOCW_SERVER_ACEPTADOR_H
#define THREADS_2023C2_TINCHOCW_SERVER_ACEPTADOR_H

#include <list>
#include <utility>

#include "common_liberror.h"
#include "common_sockets.h"
#include "common_thread.h"
#include "server_jugador.h"
#include "server_monitor_de_queues.h"

class Aceptador: public Thread {
private:
    Socket& skt;
    std::list<Jugador*> jugadores;
    bool seguir_aceptando;
    Monitor_de_queues lista_de_chats;


    void kill_all();
    void reap_dead();

public:
    explicit Aceptador(Socket& acept);
    virtual void run() override;
    void kill();

    // No permito la copia del aceptador
    Aceptador(const Aceptador&) = delete;
    Aceptador& operator=(const Aceptador&) = delete;
    ~Aceptador() override;
};
#endif  // THREADS_2023C2_TINCHOCW_SERVER_ACEPTADOR_H
