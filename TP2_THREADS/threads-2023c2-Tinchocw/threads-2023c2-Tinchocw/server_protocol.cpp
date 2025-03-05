#include "server_protocol.h"

Protocolo_servidor::Protocolo_servidor(Socket&& jugador):
        jugador(std::move(jugador)), was_closed(false), keep_talking(true) {}

bool Protocolo_servidor::estoy_conectado() {
    /*
     * IMPORTANTE: Ver que el booleano que consulto desde afuera para ver la condición del socket es
     * el atomico para poder consultarlo y ver su versión más actualizada. Puede suceder el caso en
     * el que accedo con el hilo reciever y se cierra el socket y por lo tanto lo cambio y ya se que
     * no estoy más conectado, pero puede suceder que en el medio cambio el hilo y hago un send y
     * como no se llego a cambiar el valor del was_closed yo pienso que esta abierto, pero en
     * realidad no, entonces se me estaría generando una race condition.
     * */
    return keep_talking;
}

void Protocolo_servidor::recibir_mensaje(struct mensaje& msj) {

    char comando;
    recibir_y_chequear(&comando, ONE_BYTE);
    if (!was_closed) {
        uint16_t len_argumento = 0;
        recibir_mensaje_de_dos_bytes(len_argumento);

        std::vector<char> argumento_recibido_net(len_argumento);
        recibir_y_chequear(argumento_recibido_net.data(), len_argumento);

        if (comando == CHAT_NET_CLIENT || comando == CHAT_NET_SERVER) {
            msj.comando = CHAT;
        }

        std::string argumento_recibido(argumento_recibido_net.begin(),
                                       argumento_recibido_net.end());
        msj.argumento = argumento_recibido;
    }
}

void Protocolo_servidor::enviar_cantidad_de_jugadores(struct mensaje msj) {
    unsigned char cantidad_de_jugadores_conectados_net = stoi(msj.argumento);
    char comando_net = PLAYER_CANT_NET;

    enviar_y_chequear(&comando_net, ONE_BYTE);
    enviar_y_chequear(&cantidad_de_jugadores_conectados_net, ONE_BYTE);
}

void Protocolo_servidor::reenviar_mensaje_jugador(struct mensaje msj) {

    char comando_net = CHAT_NET_SERVER;
    uint16_t len_argumento = 0;

    std::vector<char> mensaje(msj.argumento.begin(), msj.argumento.end());
    len_argumento = mensaje.size();

    enviar_y_chequear(&comando_net, ONE_BYTE);
    enviar_mensaje_de_dos_bytes(len_argumento);
    enviar_y_chequear(mensaje.data(), len_argumento);
}

void Protocolo_servidor::enviar_mensaje(struct mensaje msj) {
    if (msj.comando == CHAT) {
        reenviar_mensaje_jugador(msj);

    } else if (msj.comando == PLAYER_CANT) {
        enviar_cantidad_de_jugadores(msj);
    }
}

void Protocolo_servidor::desconectar_jugador() {
    jugador.shutdown(2);  // No hay más envío ni recepción de mensajes
    jugador.close();
}

void Protocolo_servidor::enviar_y_chequear(const void* data, unsigned int sz) {

    jugador.sendall(data, sz, &was_closed);
    if (was_closed) {
        keep_talking = false;
    }
}

void Protocolo_servidor::recibir_y_chequear(void* data, unsigned int sz) {
    if (!was_closed) {
        jugador.recvall(data, sz, &was_closed);
    }
    if (was_closed) {
        keep_talking = false;
    }
}

void Protocolo_servidor::recibir_mensaje_de_dos_bytes(uint16_t& len_argumento) {

    unsigned char len_argumento_net[TWO_BYTES];
    recibir_y_chequear(&len_argumento_net, TWO_BYTES);
    memcpy(&len_argumento, len_argumento_net, TWO_BYTES);
    len_argumento = ntohs(len_argumento);
}

void Protocolo_servidor::enviar_mensaje_de_dos_bytes(uint16_t len_argumento) {
    len_argumento = htons(len_argumento);
    enviar_y_chequear(&len_argumento, TWO_BYTES);
}
