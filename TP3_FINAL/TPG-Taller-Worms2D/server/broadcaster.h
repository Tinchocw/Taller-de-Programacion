#ifndef BROADCASTER_H
#define BROADCASTER_H

#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <string>

#include "../common/queue.h"

#include "info_parser.h"

class States;
class Player;
class Battlefield;

class BroadCaster {
private:
    std::mutex m;
    /*
        Este mutex es necesario ya que mientras el juego este en estado "lobby", multiples hilos
       intentan acceder al mapa.
    */
    std::map<uint8_t, Queue<std::shared_ptr<States>>*> broadcast_map;
    InfoParser infoParser;

    void broadcast(const std::list<std::shared_ptr<States>>& game_states);

public:
    explicit inline BroadCaster(Game& game): infoParser(game) {}
    /*
        @param id: ID del jugador
        @param state_queue: state queue del jugador

        @brief aniade la queue de states de un cliente para broadcastear los states
    */
    void add_queue(const uint8_t& id, Queue<std::shared_ptr<States>>& state_queue);
    /*
        @brief broadcastea los states de los jugadores en lobby
    */
    void broadcastLobby();
    /*
        @param map_name: Nombre del mapa en curso

        @brief broadcastea como esta compuesto el mapa y que mapa es
    */
    void broadcastLevelLayout(const std::string& map_name);
    /*
        @param id_of_active_player: ID del jugador que tiene el turno

        @brief Envia los states del juego en curso, proyectiles, worms, players, etc..
    */
    void broadcastGame(const uint8_t& id_of_active_player);
    /*
        @param ready_count: Referencia a la cantidad de jugadores ready
        @param players: Mapa de jugadores en el juego

        @brief elimina los jugadores desconectados
    */
    void remove_closed_clients(uint8_t& ready_count,
                               std::map<uint8_t, std::unique_ptr<Player>>& players);
    /*
        @param player_id: ID del jugador a eliminar

        @brief Elimina un jugador del lobby
    */
    void removeLobbyPlayer(const uint8_t& player_id);
    /*
        @param player_turn: ID del jugador con el turno
        @param block_input: Si es verdadero envia a todos los jugadores que ninguno posee el turno

        @brief Envia el estado del turno a todos los jugadores, informando quien tiene el turno
    */
    void broadcast_turn(const uint8_t& player_turn, const bool& block_input = false);
    /*
        @param players: Mapa de jugadores en el juego

        @brief Envia a el jugador que esta vivo que gano y a los demas que perdieron
    */
    void broadcastWin(std::map<uint8_t, std::unique_ptr<Player>>& players);

    ~BroadCaster() = default;
};


#endif
