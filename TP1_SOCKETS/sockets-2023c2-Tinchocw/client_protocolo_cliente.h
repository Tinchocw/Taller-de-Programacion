#ifndef SOCKETS_2023C2_TINCHOCW_COMMON_DECODIFICADOR_H
#define SOCKETS_2023C2_TINCHOCW_COMMON_DECODIFICADOR_H

#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <arpa/inet.h>

#include "common_sockets.h"

#define FALLO "F"
#define EXITO '\000'
#define ERROR '\001'

#define SELECT_ '\001'
#define DIR_ '\003'
#define MOV_ '\004'
#define JUMP_ '\005'


class Protocolo_cliente {
private:
    const std::string hostname;
    std::vector<char> mensaje_decodificado;
    Socket skt;
    bool es_select;

    std::vector<std::string> instruccion_select_recibida();
    void recibir_coordenada(std::vector<std::string>& coordenada);

public:
    explicit Protocolo_cliente(const std::string& hostname, const std::string& servname = "http");

    void procesar_select(std::string argumento);
    void procesar_dir(std::string argumento);
    void procesar_mov();
    void procesar_jump(std::string argumento);
    std::vector<std::string> instruccion_recibida();
};
#endif  // SOCKETS_2023C2_TINCHOCW_COMMON_DECODIFICADOR_H
