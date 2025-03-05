#ifndef SOCKETS_2023C2_TINCHOCW_GUSANO_SERVIDOR_H
#define SOCKETS_2023C2_TINCHOCW_GUSANO_SERVIDOR_H

#include <iostream>

#define DERECHA 'D'
#define IZQUIERDA 'I'

class Gusano {
private:
    int x;
    int y;
    char orientacion;

public:
    Gusano(int x, int y, char orientacion);

    void avanzar();
    void avanzar_derecha();
    void avanzar_izquierda();
    void saltar_hacia_adelante();
    void saltar_hacia_atras();
    void rotar_hacia_derecha();
    void rotar_hacia_izquierda();
    void caer_una_posicion();
    void saltar_hacia_arriba();

    uint32_t coordenada_x();  // Es la opción más rapida, no me parece que sea la mejor
    uint32_t coordenada_y();

    bool orientacion_derecha();
    bool orientacion_izquierda();
};


#endif  // SOCKETS_2023C2_TINCHOCW_GUSANO_SERVIDOR_H
