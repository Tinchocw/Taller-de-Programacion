#ifndef THREADS_2023C2_TINCHOCW_SERVER_PROTOCOLO_H
#define THREADS_2023C2_TINCHOCW_SERVER_PROTOCOLO_H

#include <algorithm>
#include <atomic>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "common_constantes_protocolo.h"
#include "common_sockets.h"

class Protocolo_servidor {
private:
    Socket jugador;
    bool was_closed;
    std::atomic<bool> keep_talking;

    void enviar_y_chequear(const void* data, unsigned int sz);
    void recibir_y_chequear(void* data, unsigned int sz);
    void recibir_mensaje_de_dos_bytes(uint16_t& len_argumento);
    void enviar_mensaje_de_dos_bytes(uint16_t len_argumento);
    void enviar_cantidad_de_jugadores(struct mensaje msj);
    void reenviar_mensaje_jugador(struct mensaje msj);

public:
    explicit Protocolo_servidor(Socket&& jugador);
    bool estoy_conectado();
    void recibir_mensaje(struct mensaje& msj);
    void enviar_mensaje(struct mensaje msj);
    void desconectar_jugador();
    //~Protocolo_servidor(); //VER SI IMPLEMENTAR EL CERRADO DEL SOCKET
};


#endif  // THREADS_2023C2_TINCHOCW_SERVER_PROTOCOLO_H
