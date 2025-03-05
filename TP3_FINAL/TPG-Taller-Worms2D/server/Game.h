#ifndef GAME_H
#define GAME_H

#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "../common/States.h"
#include "../common/config.h"
#include "../common/const.h"
#include "../common/queue.h"
#include "box2d/box2d.h"

#include "Player.h"
#include "battlefield.h"
#include "broadcaster.h"
#include "game_loop.h"
#include "worm_handler.h"

#define EXTRA_LIFE Config::yamlNode["extra_worm_life"].as<int>()
#define MAX_PLAYERS Config::commonNode["max_players"].as<unsigned int>()
#define WORMS_QUANTITY Config::commonNode["worms_quantity"].as<unsigned int>()

class Projectile;

class Game {
private:
    const std::string description;
    const std::string map_name;
    const uint8_t game_id;

    Battlefield battlefield;
    std::mutex m;
    /*
        Este mutex es necesario ya que mientras el juego este en estado "lobby", multiples hilos
       intentan acceder al mapa.
    */
    std::map<uint8_t, std::unique_ptr<Player>> players;
    uint8_t ready_count;

    uint8_t worm_counter;
    std::vector<b2Vec2> spawn_points;

    WormHandler worm_handler;
    BroadCaster broadcaster;

    GameLoop gameloop;
    bool need_to_join_loop;

    /*
        @brief Notifica el estado del lobby
     */
    void notify_lobby_state();

    /*
        @brief Retorna verdadero en el caso de que haya más de un jugador jugando, falso en caso
       contrario
     */
    bool non_locking_is_playing();

    /*
        @brief Aplica posición inicial a todos los worms
     */
    void spawnWorms();

public:
    explicit inline Game(std::string desc, const std::string& map, const uint8_t& game_id,
                         Queue<uint8_t>& erase_id_queue):
            description(std::move(desc)),
            map_name(map),
            game_id(game_id),
            battlefield(map_name),
            ready_count(0),
            worm_counter(0),
            worm_handler(players),
            broadcaster(*this),
            gameloop(*this, this->game_id, erase_id_queue),
            need_to_join_loop(false) {

        for (size_t i = 0; i < WORMS_QUANTITY;) {
            for (const auto& element: Config::spawnLayoutNode[map_name]) {
                // Extract values from the YAML node
                auto x = element[0].as<float>();
                auto y = element[1].as<float>();
                spawn_points.emplace_back(x, y);
                ++i;
            }
        }
    }

    /*
        @brief Retorna la action_queue
     */
    Queue<std::shared_ptr<PlayerAction>>& get_action_queue();

    /*
        @brief Retorna la información del juego
     */
    std::shared_ptr<GameInfoL> getInfo();

    /*
        param state_queue: Queue a ser agregada
        @brief  Agrega un nuevo cliente a la client_queue
     */
    void add_client_queue(const uint8_t& id, Queue<std::shared_ptr<States>>& state_queue);

    /*
        @param player_id: Id del jugador a ser removido
        @brief Remueve al jugador de Lobby de la ip pasada por parámetro
     */
    void removeLobbyPlayer(const uint8_t& player_id);

    /*
        @brief Retorna verdadero en caso de no haber jugadores, falso en caso contrario
     */
    bool isEmpty();

    /*
        @brief Retorna verdadero en el caso de que haya más de un jugador jugando, falso en caso
       contrario
     */
    bool is_playing();

    /*
        @brief Setea al jugador con id pasado por parámetro ready
     */
    void set_player_ready(const uint8_t& id);

    ~Game();

    friend class GameLoop;
    friend class InfoParser;
};


#endif  // GAME_H
