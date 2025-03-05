#ifndef ACTORHOLDER_H
#define ACTORHOLDER_H

#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "GameActor.h"

class ActorHolder {
private:
    std::unordered_map<uint8_t, std::shared_ptr<GameActor>> active_actors;
    std::unordered_map<uint8_t, std::pair<std::shared_ptr<States>, std::shared_ptr<GameActor>>>
            inactive_actors;

public:
    ActorHolder() = default;

    /*
        @brief Retorna verdadero en caso de que el actor con el id dado se encuentre cargado,
        falso en caso contrario
    */
    bool actor_loaded(const uint8_t& actor_id);

    /*
        @brief Agrega un nuevo actor al holder
    */
    void add_actor(const uint8_t& actor_id, std::shared_ptr<GameActor> new_actor);

    /*
        @brief Actualiza el estado del actor con el id dado
    */
    void update_actor_state(uint8_t actor_id, std::shared_ptr<States>& actor_state);

    /*
        @brief Remueve el actor con el id dado al mapa de actores inactivos
    */
    void remove_actor(const uint8_t& actor_id, std::shared_ptr<States> final_state);

    /*
        @brief Ejecuta el estado de todos los actores
    */
    void play_actors_state(std::shared_ptr<SDL2pp::Renderer>& game_renderer,
                           AudioPlayer& effects_player);

    /*
        @brief Imprime el estado de todos los actores
    */
    void print_actors_state(std::shared_ptr<SDL2pp::Renderer>& game_renderer,
                            TextPrinter& state_printer);

    /*
        @brief Elimina todos los actores inactivos del mapa de inactivos
    */
    void delete_inactive_actors();
};


#endif  // ACTORHOLDER_H
