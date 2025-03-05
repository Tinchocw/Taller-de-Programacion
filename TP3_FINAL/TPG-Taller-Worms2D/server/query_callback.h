#ifndef WORMS2D_QUERY_CALLBACK_H
#define WORMS2D_QUERY_CALLBACK_H

#include <vector>

#include <box2d/box2d.h>

class Query_callback: public b2QueryCallback {
private:
    std::vector<b2Body*> foundBodies;

public:
    bool ReportFixture(b2Fixture* fixture) override;

    /*
        @brief Retorna la cantidad de cuerpos encontrados
     */
    int found_bodies_size();

    /*
        @param i: Posición del vector del cuerpo buscado

        @brief Retorna el cuerpo encontrado por en la posición pasada por parámetro
    */
    b2Body* found_bodie_at(const int& i);
};


#endif  // WORMS2D_QUERY_CALLBACK_H
