#ifndef LEVEL_HOLDER_H
#define LEVEL_HOLDER_H

#include <list>
#include <memory>
#include <string>

#include "../common/States.h"

#include "bar.h"

class Engine;

class LevelHolder {
private:
    Battlefield& battlefield_ref;
    std::list<Bar> bars;

public:
    explicit LevelHolder(Battlefield& battlefield);
    /*
        @param x: Posicion en x de la barra
        @param y: Posicion en y de la barra
        @param angle: Angulo de inclinacion de la barra respecto al centro de masa
        @param is_long: La barra es larga o es corta?

        @brief Aniade una nueva barra a la lista de barras
    */
    void add_bar(const float& x, const float& y, const float& angle, const bool& is_long);
    /*
        @param map_name: Nombre del mapa en curso

        @brief Lee todas las barras del archivo y las aniade a la lista
    */
    std::shared_ptr<LevelStateG> get_level_building_state(const std::string& map_name);
};


#endif  // LEVEL_HOLDER_H
