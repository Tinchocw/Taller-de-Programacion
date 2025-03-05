#include "server_protocolo_servidor.h"

Protocolo_servidor::Protocolo_servidor(Socket&& cliente): cliente(std::move(cliente)) {}

void Protocolo_servidor::enviar_respuesta(char codigo_instruccion,
                                          std::vector<std::string> respuesta_servidor) {
    bool was_closed = false;
    uint32_t coordenada_x;
    uint32_t coordenada_y;


    if (codigo_instruccion == SELECT) {

        char codigo_habilitador = stoi(respuesta_servidor.at(0));

        coordenada_x = stoi(respuesta_servidor.at(1));
        coordenada_y = stoi(respuesta_servidor.at(2));
        cliente.sendall(&codigo_habilitador, 1, &was_closed);

    } else {
        coordenada_x = stoi(respuesta_servidor.at(0));
        coordenada_y = stoi(respuesta_servidor.at(1));
    }

    coordenada_x = htonl(coordenada_x);
    coordenada_y = htonl(coordenada_y);


    uint32_t x;
    uint32_t y;


    memcpy(&x, &coordenada_x, sizeof(x));
    memcpy(&y, &coordenada_y, sizeof(y));

    cliente.sendall(&x, 4, &was_closed);
    cliente.sendall(&y, 4, &was_closed);
}


void Protocolo_servidor::recibir_instrucciones(char& codigo_instruccion, std::string& argumento,
                                               bool& was_closed) {

    cliente.recvall(&codigo_instruccion, 1, &was_closed);

    if (codigo_instruccion == SELECT_) {
        unsigned char len_instruccion_net[2];
        cliente.recvall(&len_instruccion_net, 2, &was_closed);

        uint16_t len_instruccion = 0;
        memcpy(&len_instruccion, len_instruccion_net, sizeof(uint16_t));
        len_instruccion = ntohs(len_instruccion);

        std::vector<char> buf_nombre_mapa;
        buf_nombre_mapa.resize(len_instruccion);

        cliente.recvall(buf_nombre_mapa.data(), len_instruccion, &was_closed);

        argumento.append(buf_nombre_mapa.begin(), buf_nombre_mapa.end());
        codigo_instruccion = SELECT;

    } else if (codigo_instruccion == DIR_) {
        char buf_movimiento;
        cliente.recvall(&buf_movimiento, 1, &was_closed);

        if (buf_movimiento == ROTAR_IZQUIERDA_) {
            argumento = ROTAR_A_IZQUIERDA;
        } else if (buf_movimiento == ROTAR_DERECHA_) {
            argumento = ROTAR_A_DERECHA;
        }
        codigo_instruccion = DIR;
    } else if (codigo_instruccion == JUMP_) {
        char buf_movimiento;
        cliente.recvall(&buf_movimiento, 1, &was_closed);
        if (buf_movimiento == SALTAR_ADELANTE_) {
            argumento = SALTAR_HACIA_ADELANTE;
        } else if (buf_movimiento == SALTAR_ATRAS_) {
            argumento = SALTAR_HACIA_ATRAS;
        }
        codigo_instruccion = JUMP;
    } else if (codigo_instruccion == MOV_) {
        codigo_instruccion = MOV;
    }
}
