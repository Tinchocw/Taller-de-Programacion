#ifndef SOCKETS_2023C2_TINCHOCW_CLIENT_INTERPRETE_H
#define SOCKETS_2023C2_TINCHOCW_CLIENT_INTERPRETE_H

#include <string>

#include "client_protocolo_cliente.h"

#define SELECT "select"
#define MOV "move"
#define JUMP "jump"
#define DIR "dir"

class Interprete {
public:
    void preparar_intruccion_a_enviar(Protocolo_cliente& protocolo, std::string mensaje_entrada);
};


#endif  // SOCKETS_2023C2_TINCHOCW_CLIENT_INTERPRETE_H
