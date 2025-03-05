#ifndef WORMS2D_GB_H
#define WORMS2D_GB_H

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../common/queue.h"
#include "../common/thread.h"

#include "player_action.h"
#include "sclient.h"

class Game;

class GBCleaner: public Thread {
private:
    Queue<uint8_t> game_id_to_clean;
    GameBrowser& gb;
    std::atomic<bool> killed;

public:
    explicit inline GBCleaner(GameBrowser& gb): Thread(), gb(gb), killed(false) {}

    /*
        @brief Termina de forma forzosa el Cleaner
     */
    void kill();

    /*
        @brief Corre el Cleaner esperando a popear una id de un juego a eliminar
    */
    void run() override;

    ~GBCleaner() override = default;

    friend class GameBrowser;
};


class GameBrowser {
private:
    std::mutex m;
    std::map<uint8_t, std::shared_ptr<Game>> games;
    uint8_t game_id_count;
    GBCleaner cleaner;

    /*
        @param game_id: id de la partida a ser eliminada
        @brief Elimina la partida determinada por el id pasado por parámetro
    */
    void erase_game(const uint8_t& game_id);

public:
    /*
        @brief Inicializa el conteo de las id de GameLoop en 0 (game_id_count)
    */
    GameBrowser();

    /*
        @param desc: Descripción de la partida
        @param map: Nombre del mapa de la partida
        @param game_id_to_create: Id de la partida a crear

        @brief Crea una nueva partida con el id, descripción y mapa pasados por parámetro
    */
    void create_game(const std::string& desc, const std::string& map, int16_t& game_id_to_create);

    /*
        @param game_code: Código de la partida
        @param id: Id del cliente a ser agregado a la partida
        param state_queue: queue del cliente a ser agregado

        @brief Agrega client al GameLoop especificado por game_code
    */
    void join_game(const uint8_t& game_code, const uint8_t& id,
                   Queue<std::shared_ptr<States>>& state_queue);


    /*
        @param player_id: Id de jugador a ser removido
        @param game_id: Id de partida a la cual el jugador es removido

        @brief Remueve a jugador con ip pasada por parámetro de la partida de ip pasada por
       parámetro
    */
    void removeLobbyPlayer(const uint8_t& player_id, const uint8_t& game_id);

    /*
        @param player_id: Id de jugador a ser seteado ready
        @param game_id: Id de partida

        @brief Setea al jugador con ip pasado por parámetro ready en la partida con id pasado por
       parámetro

    */
    void set_player_ready(const uint8_t& id, const uint8_t& id_game);

    /*
        @param game_id: Id de la partida

        @brief Retorna, en el caso de existir, la action queue de la partida con id pasado por
       parámetro
    */
    Queue<std::shared_ptr<PlayerAction>>& getQueue(const uint8_t& game_id);

    /*
        @brief Recolecta la información de los juegos en ejecución
    */
    void infoGames(std::vector<std::shared_ptr<GameInfoL>>&);

    /*
        @param game_id: Id de la partida

        @brief Si la partida existe retorna si estan jugando, false en otro caso
    */
    const bool game_started_playing(const uint8_t& game_id);

    /*
        @brief Joinea los hilos
    */
    ~GameBrowser();
    /*
     *  No queremos ni copiar ni mover el monitor
     */
    GameBrowser(const GameBrowser&) = delete;
    GameBrowser& operator=(const GameBrowser&) = delete;

    GameBrowser(GameBrowser&&) = delete;
    GameBrowser& operator=(GameBrowser&&) = delete;

    friend class GBCleaner;
};


#endif  // WORMS2D_GB_H
