#ifndef INFO_PARSER_H
#define INFO_PARSER_H

#include <list>
#include <memory>
#include <string>

class States;
class Game;


class InfoParser {
private:
    Game& game;
    /*
        Lock necesario unicamente para la parte del lobby
    */

public:
    explicit inline InfoParser(Game& game): game(game) {}

    /*
        @param states: Referencia a la lista de states a llenar

        @brief Pushea en la lista de states todos los states de lobby que puede para luego enviarlos
     */
    void makeLobbyState(std::list<std::shared_ptr<States>>& states);

    /*
        @param states: Referencia a la lista de states a llenar
        @param id_of_active_player: ID del jugador con el turno

        @brief Pushea en la lista de states todos los states de game que puede para luego enviarlos
     */
    void makeGameState(std::list<std::shared_ptr<States>>& states,
                       const uint8_t& id_of_active_player);

    /*
        @param states: Referencia a la lista de states a llenar
        @param map_name: Nombre del mapa en curso

        @brief Pushea en la lista de states todos los states de game (especializados en el mapa)
            que puede para luego enviarlos
     */
    void makeLevelState(std::list<std::shared_ptr<States>>& states, const std::string& map_name);
};


#endif
