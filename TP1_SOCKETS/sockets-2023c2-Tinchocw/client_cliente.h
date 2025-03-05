#ifndef SOCKETS_2023C2_TINCHOCW_COMMON_CLIENT_H
#define SOCKETS_2023C2_TINCHOCW_COMMON_CLIENT_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <arpa/inet.h>

#include "client_interprete.h"
#include "client_protocolo_cliente.h"
#include "common_printer.h"

#define FALLO "F"


class Cliente {
private:
    Protocolo_cliente protocolo;
    Printer printer;
    Interprete interprete;

    void mostrar_respuesta_a_instruccion(std::vector<std::string> respuesta);

public:
    Cliente(const std::string& hostname, const std::string& servname);
    void realizar_movimientos();
};


#endif  // SOCKETS_2023C2_TINCHOCW_COMMON_CLIENT_H
