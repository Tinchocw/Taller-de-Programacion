#include "game_loop.h"

#include <chrono>
#include <utility>

#include <spdlog/spdlog.h>

#include "Game.h"
#include "player_action.h"
#include "sclient.h"
#include "turn_handler.h"

Queue<std::shared_ptr<PlayerAction>>& GameLoop::get_action_queue() { return this->action_queue; }

void GameLoop::run() {
    std::chrono::duration<float> elapsed_seconds = std::chrono::duration<float>(0);
    TurnHandler turn_handler(game.players, game.broadcaster, game.worm_handler, game.battlefield);

    try {
        while (game.non_locking_is_playing()) {
            std::chrono::time_point<std::chrono::steady_clock> before =
                    std::chrono::steady_clock::now();

            ActualTurn turn_id = turn_handler.updateTurn(elapsed_seconds);

            std::shared_ptr<PlayerAction> c;
            if (action_queue.try_pop(c)) {
                c->execute(game.worm_handler, turn_id.player_id, turn_handler);
            }

            game.worm_handler.check_drown_worms();
            game.broadcaster.broadcastGame(turn_id.player_id);
            game.worm_handler.checkDeadWorms();
            game.battlefield.remove_dead_objects();

            game.battlefield.step();
            game.worm_handler.update_physics();
            game.worm_handler.update_weapon(turn_handler);
            game.worm_handler.check_falling_worms();

            game.broadcaster.remove_closed_clients(game.ready_count, game.players);


            std::chrono::time_point<std::chrono::steady_clock> after =
                    std::chrono::steady_clock::now();

            elapsed_seconds = std::chrono::milliseconds(EST_TICK_TIME) - (after - before);

            std::this_thread::sleep_for(elapsed_seconds);
        }
    } catch (const SomeOneWins& e) {}

    game.broadcaster.broadcastWin(game.players);

    erase_id_queue.push(this->game_id);
}
