#ifndef SOCKETS_2023C2_TINCHOCW_SERVIDORPROTOCOLO_H
#define SOCKETS_2023C2_TINCHOCW_SERVIDORPROTOCOLO_H

#include <cstring>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "common_sockets.h"

#define SELECT 'S'
#define DIR 'D'
#define MOV 'M'
#define JUMP 'J'

#define ROTAR_A_DERECHA "D"
#define ROTAR_A_IZQUIERDA "I"

#define SALTAR_HACIA_ADELANTE "A"
#define SALTAR_HACIA_ATRAS "B"

#define SELECT_ '\001'
#define DIR_ '\003'
#define MOV_ '\004'
#define JUMP_ '\005'

#define ROTAR_IZQUIERDA_ '\000'
#define ROTAR_DERECHA_ '\001'
#define SALTAR_ADELANTE_ '\000'
#define SALTAR_ATRAS_ '\001'

class Protocolo_servidor {
private:
    Socket cliente;

public:
    explicit Protocolo_servidor(Socket&& cliente);
    void recibir_instrucciones(char& codigo_instruccion, std::string& argumento, bool& was_closed);
    void enviar_respuesta(char codigo_instruccion, std::vector<std::string> respuesta_servidor);
};


#endif  // SOCKETS_2023C2_TINCHOCW_SERVIDORPROTOCOLO_H
