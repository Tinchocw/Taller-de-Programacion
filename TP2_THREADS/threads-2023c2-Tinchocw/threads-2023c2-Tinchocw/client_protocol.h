#ifndef THREADS_2023C2_TINCHOCW_CLIENT_PROTOCOL_H
#define THREADS_2023C2_TINCHOCW_CLIENT_PROTOCOL_H

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include <arpa/inet.h>

#include "common_constantes_protocolo.h"
#include "common_printer.h"
#include "common_sockets.h"

class Protocolo_cliente {

private:
    Socket skt;
    bool was_closed;

    void recibir_cantidad_de_jugadores(struct mensaje& msj);
    void recibir_mensaje_de_otro_jugador(struct mensaje& mensaje_recibido);
    void recibir_numero_de_dos_bytes(uint16_t& len_mensaje);
    void enviar_numero_de_dos_bytes(uint16_t len_mensaje);

public:
    Protocolo_cliente(const std::string& hostname, const std::string& servname);
    void recibir_instruccion(struct mensaje& mensaje_input);
    void enviar_chat(struct mensaje msj);
    void desconectar_jugador();
    bool esta_conectado();
};


#endif  // THREADS_2023C2_TINCHOCW_CLIENT_PROTOCOL_H
