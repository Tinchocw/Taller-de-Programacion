#ifndef THREADS_2023C2_TINCHOCW_SERVER_SERVIDOR_H
#define THREADS_2023C2_TINCHOCW_SERVER_SERVIDOR_H

#include <iostream>
#include <string>
#include <utility>

#include "common_sockets.h"
#include "server_aceptador.h"


class Servidor {
    Aceptador* aceptador;
    Socket acept;


public:
    explicit Servidor(Socket&& acept_);
    void run();
    Servidor(const Servidor&) = delete;
    Servidor& operator=(const Servidor&) = delete;
    ~Servidor();
};


#endif  // THREADS_2023C2_TINCHOCW_SERVER_SERVIDOR_H
