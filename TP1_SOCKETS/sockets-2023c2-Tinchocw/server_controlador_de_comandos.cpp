#include "server_controlador_de_comandos.h"

void Controlador_de_comandos::controlar_comandos_a_juego(Juego& juego, char codigo_de_mensaje,
                                                         std::string const argumento) {
    if (codigo_de_mensaje == SELECT) {
        juego.seleccionar_mapa(argumento);
    } else if (codigo_de_mensaje == DIR) {
        if (argumento == ROTAR_A_IZQUIERDA) {
            juego.gusano_orientar_izquierda();
        } else if (argumento == ROTAR_A_DERECHA) {
            juego.gusano_orientar_derecha();
        }
    } else if (codigo_de_mensaje == MOV) {
        juego.gusano_moverse();
    } else if (codigo_de_mensaje == JUMP) {
        if (argumento == SALTAR_HACIA_ADELANTE) {
            juego.gusano_saltar_hacia_adelante();
        } else if (argumento == SALTAR_HACIA_ATRAS) {
            juego.gusano_saltar_hacia_atras();
        }
    }
}

void Controlador_de_comandos::controlar_respuesta_de_juego(
        Juego& juego, char codigo_de_mensaje, std::vector<std::string>& respuesta_juego) {
    if (codigo_de_mensaje == SELECT) {
        respuesta_juego = juego.respuesta_a_intruccion_select();
    } else {
        respuesta_juego = juego.coordenada_actual_gusano();
    }
}
