#ifndef SOCKETS_2023C2_TINCHOCW_SERVER_CONTROLADOR_DE_COMANDOS_H
#define SOCKETS_2023C2_TINCHOCW_SERVER_CONTROLADOR_DE_COMANDOS_H

#include <string>
#include <vector>

#include "server_juego.h"

#define SELECT 'S'
#define DIR 'D'
#define MOV 'M'
#define JUMP 'J'

#define ROTAR_A_DERECHA "D"
#define ROTAR_A_IZQUIERDA "I"

#define SALTAR_HACIA_ADELANTE "A"
#define SALTAR_HACIA_ATRAS "B"


class Controlador_de_comandos {
public:
    void controlar_comandos_a_juego(Juego& juego, char codigo_de_mensaje, std::string argumento);
    void controlar_respuesta_de_juego(Juego& juego, char codigo_de_mensaje,
                                      std::vector<std::string>& respuesta_juego);
};

#endif  // SOCKETS_2023C2_TINCHOCW_SERVER_CONTROLADOR_DE_COMANDOS_H
