#include "server_gusano.h"

Gusano::Gusano(int coordenada_x, int coordenada_y, char orientacion_) {
    x = coordenada_x;
    y = coordenada_y;
    orientacion = orientacion_;
}

void Gusano::avanzar() {
    if (orientacion_derecha()) {
        x++;
    } else if (orientacion_izquierda()) {
        x--;
    }
}
void Gusano::avanzar_derecha() { x++; }
void Gusano::avanzar_izquierda() { x--; }

void Gusano::rotar_hacia_derecha() { orientacion = DERECHA; }
void Gusano::rotar_hacia_izquierda() { orientacion = IZQUIERDA; }

void Gusano::saltar_hacia_adelante() {
    if (orientacion_derecha()) {
        x = x + 2;
        y = y - 1;
    } else if (orientacion_izquierda()) {
        x = x - 2;
        y = y - 1;
    }
}
void Gusano::saltar_hacia_atras() {
    if (orientacion_derecha()) {
        x = x - 1;
        y = y - 2;
    } else if (orientacion_izquierda()) {
        x = x + 1;
        y = y - 2;
    }
}
void Gusano::caer_una_posicion() { y++; }
void Gusano::saltar_hacia_arriba() { y--; }

bool Gusano::orientacion_derecha() { return orientacion == DERECHA; }
bool Gusano::orientacion_izquierda() { return orientacion == IZQUIERDA; }

uint32_t Gusano::coordenada_x() { return x; }
uint32_t Gusano::coordenada_y() { return y; }
