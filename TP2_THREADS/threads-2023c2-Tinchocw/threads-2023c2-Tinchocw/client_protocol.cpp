#include "client_protocol.h"

Protocolo_cliente::Protocolo_cliente(const std::string& hostname, const std::string& servname):
        skt(hostname.c_str(), servname.c_str()), was_closed(false) {}
void Protocolo_cliente::enviar_chat(struct mensaje msj) {

    char comando_net = CHAT_NET_CLIENT;
    std::vector<char> mensaje(msj.argumento.begin(), msj.argumento.end());
    uint16_t len_argumento = mensaje.size();

    skt.sendall(&comando_net, ONE_BYTE, &was_closed);
    enviar_numero_de_dos_bytes(len_argumento);
    skt.sendall(mensaje.data(), len_argumento, &was_closed);
}

void Protocolo_cliente::recibir_instruccion(struct mensaje& mensaje_input) {

    char comando;
    skt.recvall(&comando, ONE_BYTE, &was_closed);
    if (comando == CHAT_NET_SERVER) {
        mensaje_input.comando = CHAT;
        recibir_mensaje_de_otro_jugador(mensaje_input);

    } else if (comando == PLAYER_CANT_NET) {
        mensaje_input.comando = PLAYER_CANT;
        recibir_cantidad_de_jugadores(mensaje_input);
    }
}

void Protocolo_cliente::recibir_mensaje_de_otro_jugador(struct mensaje& mensaje_recibido) {

    uint16_t len_mensaje = 0;
    recibir_numero_de_dos_bytes(len_mensaje);

    std::vector<char> mensaje_recibido_net(len_mensaje);
    skt.recvall(mensaje_recibido_net.data(), len_mensaje, &was_closed);

    std::string argumento_recibido(mensaje_recibido_net.begin(), mensaje_recibido_net.end());
    mensaje_recibido.argumento = argumento_recibido;
}

void Protocolo_cliente::recibir_cantidad_de_jugadores(struct mensaje& mensaje_recibido) {
    unsigned char cantidad_de_jugadores_net = 0;

    skt.recvall(&cantidad_de_jugadores_net, ONE_BYTE, &was_closed);
    int cantidad_de_jugadores = cantidad_de_jugadores_net;
    mensaje_recibido.argumento = std::to_string(cantidad_de_jugadores);
}

void Protocolo_cliente::desconectar_jugador() { was_closed = true; }

bool Protocolo_cliente::esta_conectado() { return not was_closed; }

void Protocolo_cliente::recibir_numero_de_dos_bytes(uint16_t& len_mensaje) {
    unsigned char len_mensaje_net[TWO_BYTES];

    skt.recvall(&len_mensaje_net, TWO_BYTES, &was_closed);

    memcpy(&len_mensaje, len_mensaje_net, TWO_BYTES);
    len_mensaje = ntohs(len_mensaje);
}

void Protocolo_cliente::enviar_numero_de_dos_bytes(uint16_t len_mensaje) {
    len_mensaje = htons(len_mensaje);
    skt.sendall(&len_mensaje, TWO_BYTES, &was_closed);
}
