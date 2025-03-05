#include "server_juego.h"

Juego::Juego(std::string escenarios) {

    mapa_seleccionado = 0;
    respuesta_a_instruccion = 1;

    std::ifstream archivo_mapas;
    archivo_mapas.open(escenarios);

    if (!archivo_mapas.is_open()) {
        archivo_mapas.close();
    }

    std::string columnas, filas, nombre_mapa;

    while (!archivo_mapas.eof()) {

        archivo_mapas >> columnas >> filas >> nombre_mapa >> std::ws;
        tipos_de_mapas.push_back(nombre_mapa);
        std::vector<std::vector<char>> mapa;

        for (int i = 0; i < stoi(filas); i++) {

            std::string fila_aux;
            getline(archivo_mapas, fila_aux);
            std::vector<char> filas_(fila_aux.begin(), fila_aux.end());


            std::vector<char>::iterator it;
            it = find(filas_.begin(), filas_.end(), JUGADOR);

            if (it != filas_.end()) {
                Gusano gusano(std::distance(filas_.begin(), it), i, IZQUIERDA);
                posiciones_gusanos.push_back(gusano);
            }

            mapa.push_back(filas_);
        }
        mapas.push_back(mapa);
    }
    archivo_mapas.close();
}

bool Juego::esta_tipo_de_mapa(std::string tipo_de_mapa) {
    std::vector<std::string>::iterator it =
            find(tipos_de_mapas.begin(), tipos_de_mapas.end(), tipo_de_mapa);
    return it != tipos_de_mapas.end();
}

int Juego::selecccionar_tipo_de_mapa(std::string tipo_de_mapa) {
    std::vector<std::string>::iterator it =
            find(tipos_de_mapas.begin(), tipos_de_mapas.end(), tipo_de_mapa);
    return std::distance(tipos_de_mapas.begin(), it);
}

void Juego::seleccionar_mapa(std::string const tipo_mapa) {
    if (esta_tipo_de_mapa(tipo_mapa)) {
        mapa_seleccionado = selecccionar_tipo_de_mapa(tipo_mapa);
        respuesta_a_instruccion = 0;
    }
}
void Juego::gusano_moverse() {
    if (gusano_mira_hacia_izquierda()) {
        if (!hay_pared_a_izquierda_de_gusano()) {
            posiciones_gusanos.at(mapa_seleccionado).avanzar_izquierda();
        }
    } else if (gusano_mira_hacia_derecha()) {
        if (!hay_pared_a_derecha_de_gusano()) {
            posiciones_gusanos.at(mapa_seleccionado).avanzar_derecha();
        }
    }
    simular_gravedad();
}

void Juego::gusano_saltar_hacia_adelante() {
    bool me_movi = false;
    if (es_mov_valido(0, -1)) {
        posiciones_gusanos.at(mapa_seleccionado).saltar_hacia_arriba();
        me_movi = true;
    }
    int cont = 0;
    while (me_movi && cont < 2) {
        if (es_mov_valido(1, 0) && posiciones_gusanos.at(mapa_seleccionado).orientacion_derecha()) {
            posiciones_gusanos.at(mapa_seleccionado).avanzar_derecha();
        } else if (es_mov_valido(-1, 0) &&
                   posiciones_gusanos.at(mapa_seleccionado).orientacion_izquierda()) {
            posiciones_gusanos.at(mapa_seleccionado).avanzar_izquierda();
        }
        cont++;
    }
    simular_gravedad();
}
void Juego::gusano_saltar_hacia_atras() {

    bool me_movi = true;
    int cont = 0;
    while (me_movi && cont < 2) {
        if (es_mov_valido(0, -1)) {
            posiciones_gusanos.at(mapa_seleccionado).saltar_hacia_arriba();
            me_movi = true;
        } else {
            me_movi = false;
        }
        cont++;
    }
    if (es_mov_valido(-1, 0) && me_movi &&
        posiciones_gusanos.at(mapa_seleccionado).orientacion_derecha()) {
        posiciones_gusanos.at(mapa_seleccionado).avanzar_izquierda();
    } else if (es_mov_valido(1, 0) && me_movi &&
               posiciones_gusanos.at(mapa_seleccionado).orientacion_izquierda()) {
        posiciones_gusanos.at(mapa_seleccionado).avanzar_derecha();
    }
    simular_gravedad();
}

void Juego::gusano_orientar_derecha() {
    posiciones_gusanos.at(mapa_seleccionado).rotar_hacia_derecha();
}
void Juego::gusano_orientar_izquierda() {
    posiciones_gusanos.at(mapa_seleccionado).rotar_hacia_izquierda();
}


void Juego::simular_gravedad() {
    while (mapas.at(mapa_seleccionado)
                   .at(posiciones_gusanos.at(mapa_seleccionado).coordenada_y() + 1)
                   .at(posiciones_gusanos.at(mapa_seleccionado).coordenada_x()) != PARED) {
        posiciones_gusanos.at(mapa_seleccionado).caer_una_posicion();
    }
}

std::vector<std::string> Juego::respuesta_a_intruccion_select() {

    std::vector<std::string> solicitud_realizada;

    solicitud_realizada.push_back(std::to_string(respuesta_a_instruccion));
    std::vector<std::string> coordenada = coordenada_actual_gusano();
    solicitud_realizada.insert(solicitud_realizada.end(), coordenada.begin(), coordenada.end());

    return solicitud_realizada;
}

std::vector<std::string> Juego::coordenada_actual_gusano() {
    std::vector<std::string> coordenada;
    coordenada.push_back(std::to_string(posiciones_gusanos.at(mapa_seleccionado).coordenada_x()));
    coordenada.push_back(std::to_string(posiciones_gusanos.at(mapa_seleccionado).coordenada_y()));

    return coordenada;
}

bool Juego::es_mov_valido(int mov_en_x, int mov_en_y) {
    return mapas.at(mapa_seleccionado)
                   .at(posiciones_gusanos.at(mapa_seleccionado).coordenada_y() + mov_en_y)
                   .at(posiciones_gusanos.at(mapa_seleccionado).coordenada_x() + mov_en_x) != PARED;
}
bool Juego::gusano_mira_hacia_derecha() {
    return posiciones_gusanos.at(mapa_seleccionado).orientacion_derecha();
}
bool Juego::gusano_mira_hacia_izquierda() {
    return posiciones_gusanos.at(mapa_seleccionado).orientacion_izquierda();
}
bool Juego::hay_pared_a_derecha_de_gusano() {
    return mapas.at(mapa_seleccionado)
                   .at(posiciones_gusanos.at(mapa_seleccionado).coordenada_y())
                   .at(posiciones_gusanos.at(mapa_seleccionado).coordenada_x() + 1) == PARED;
}
bool Juego::hay_pared_a_izquierda_de_gusano() {
    return mapas.at(mapa_seleccionado)
                   .at(posiciones_gusanos.at(mapa_seleccionado).coordenada_y())
                   .at(posiciones_gusanos.at(mapa_seleccionado).coordenada_x() - 1) == PARED;
}

Juego::~Juego() {}
