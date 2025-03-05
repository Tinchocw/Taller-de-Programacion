#ifndef SOCKETS_2023C2_TINCHOCW_SERVER_SERVIDOR_H
#define SOCKETS_2023C2_TINCHOCW_SERVER_SERVIDOR_H

#include <string>
#include <utility>
#include <vector>

#include "common_printer.h"
#include "common_sockets.h"
#include "server_controlador_de_comandos.h"
#include "server_juego.h"
#include "server_protocolo_servidor.h"


class Servidor {
private:
    Protocolo_servidor protocolo;
    Controlador_de_comandos controlador_de_comandos;
    Juego juego;
    Printer printer;

public:
    Servidor(Socket&& cliente, std::string escenarios);
    void procesar_juego();
};


#endif  // SOCKETS_2023C2_TINCHOCW_SERVER_SERVIDOR_H
