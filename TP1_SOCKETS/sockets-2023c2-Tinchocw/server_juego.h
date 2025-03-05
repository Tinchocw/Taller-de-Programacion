#ifndef SOCKETS_2023C2_TINCHOCW_COMMON_SERVER_H
#define SOCKETS_2023C2_TINCHOCW_COMMON_SERVER_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "server_gusano.h"

#define SELECT 'S'
#define DIR 'D'
#define MOV 'M'
#define JUMP 'J'

#define DERECHA 'D'
#define IZQUIERDA 'I'

#define PARED 'X'
#define JUGADOR 'G'

class Juego {

private:
    std::vector<std::vector<std::vector<char>>> mapas;  // tengo un vector de mapas
    std::vector<std::string> tipos_de_mapas;
    std::vector<Gusano> posiciones_gusanos;
    int mapa_seleccionado;
    int respuesta_a_instruccion;


    bool esta_posicion_disponible(std::string posicion);
    bool es_mov_valido(int mov_en_x, int mov_en_y);
    bool hay_pared_a_izquierda_de_gusano();
    bool hay_pared_a_derecha_de_gusano();

    bool gusano_mira_hacia_derecha();
    bool gusano_mira_hacia_izquierda();


    void simular_gravedad();

    int selecccionar_tipo_de_mapa(std::string tipo_de_mapa);

public:
    explicit Juego(std::string escenarios);  // Leo el archivo y me guardo toda la información que
                                             // necesito.

    bool esta_tipo_de_mapa(std::string tipo_de_mapa);

    void seleccionar_mapa(std::string tipo_mapa);
    void gusano_moverse();
    void gusano_saltar_hacia_adelante();
    void gusano_saltar_hacia_atras();
    void gusano_orientar_derecha();
    void gusano_orientar_izquierda();

    std::vector<std::string> respuesta_a_intruccion_select();
    std::vector<std::string> coordenada_actual_gusano();

    ~Juego();
};

#endif  // SOCKETS_2023C2_TINCHOCW_COMMON_SERVER_H
