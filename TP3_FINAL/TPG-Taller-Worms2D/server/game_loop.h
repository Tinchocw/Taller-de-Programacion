#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <memory>
#include <string>

#include <stdint.h>

#include "../common/queue.h"
#include "../common/thread.h"

#include "player_action.h"

#define EST_TICK_TIME Config::yamlNode["tick_duration"].as<int>()

class Game;

class GameLoop: public Thread {
private:
    Queue<std::shared_ptr<PlayerAction>> action_queue;
    Game& game;
    const uint8_t& game_id;
    Queue<uint8_t>& erase_id_queue;

public:
    explicit inline GameLoop(Game& game, const uint8_t& game_id, Queue<uint8_t>& erase_id_queue):
            game(game), game_id(game_id), erase_id_queue(erase_id_queue) {}

     /*
        @brief Retorna la action_queue
      */
    Queue<std::shared_ptr<PlayerAction>>& get_action_queue();

    /*
        @brief Corre el game loop
     */
    void run() override;

    // Libera los recursos de todos los clientes
    ~GameLoop() override = default;

    // No queremos ni copiar ni mover el monitor
    GameLoop(const GameLoop&) = delete;
    GameLoop& operator=(const GameLoop&) = delete;

    GameLoop(GameLoop&&) = delete;
    GameLoop& operator=(GameLoop&&) = delete;
};


#endif
