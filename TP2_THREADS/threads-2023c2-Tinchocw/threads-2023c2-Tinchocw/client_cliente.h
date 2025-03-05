#ifndef THREADS_2023C2_TINCHOCW_CLIENTE_CLIENTE_H
#define THREADS_2023C2_TINCHOCW_CLIENTE_CLIENTE_H

#include <iostream>
#include <string>

#include "client_interprete.h"

class Cliente {

private:
    Interprete interprete;
    Protocolo_cliente protocolo;

    void ejecutar_input(struct mensaje msj);
    void recibir_mensajes_solicitados(struct mensaje msj);
    void mostrar_mesaje_recibido(struct mensaje msj);

public:
    Cliente(const char* hostname, const char* servername);
    void run();
};


#endif  // THREADS_2023C2_TINCHOCW_CLIENTE_CLIENTE_H
