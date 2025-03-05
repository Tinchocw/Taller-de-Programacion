#include "turn_handler.h"

#include <iostream>

#include "../common/const.h"

#include "Player.h"
#include "battlefield.h"
#include "broadcaster.h"
#include "worm_handler.h"

const TurnReset TurnHandler::need_to_update(const uint8_t players_quantity,
                                            const std::chrono::duration<float>& elapsed) {

    // Alguien gano??? Perdieron todos / empate???
    if (worm_handler.players_alive() <= 1) {
        throw SomeOneWins();
    }

    // Desconecta un player o primer turno
    if (players_quantity != current_players_quantity) {
        current_players_quantity = players_quantity;
        return advanceTurn(players_quantity);
    }

    {
        auto it = players.begin();
        std::advance(it, player_turn);

        if (current_player_worms_quantity > it->second->worms.size()) {
            return advanceTurn(players_quantity);
        }

        if (it->second->worms.empty() ? true : it->second->worms.at(worm_turn_id)->was_damaged) {
            return advanceTurn(players_quantity);
        }

        auto worm_it = it->second->worms.begin();
        advance(worm_it, it->second->worm_turn);

        if (it->second->worms.count(worm_it->first) != 1) {
            return advanceTurn(players_quantity);
        }
    }

    elapsed_time += elapsed;

    // Post turn si player_stop_action == true
    if (elapsed_time.count() < (std::pow(POST_TURN_DURATION_IN_SECONDS, player_stop_action) *
                                std::pow(TURN_DURATION_IN_SECONDS, 1 - player_stop_action))) {
        return TurnReset::NOT_RESET;
    }

    return advanceTurn(players_quantity);
}

const TurnReset TurnHandler::advanceTurn(const uint8_t& players_quantity) {
    worm_handler.stop_turn_worm();

    if (not battlefield.noProjectiles()) {
        return TurnReset::WAIT_TURN_END;
    }

    if (not worm_handler.allWormsStayStill()) {
        return TurnReset::WAIT_TURN_END;
    }

    worm_handler.clearDamagedState();
    for (++player_turn; player_turn <= players_quantity; ++player_turn) {
        // Dejamos q se pase de index para que se resetee a 0
        if (player_turn == players_quantity) {
            break;
        }

        auto it = players.begin();
        std::advance(it, player_turn);
        if (it->second->is_playing) {
            break;
        }
    }

    player_stop_action = false;
    elapsed_time = std::chrono::duration<float>(0);


    if (player_turn > (players_quantity - 1)) {
        player_turn = 0;
        for (auto& [id, player]: players) {
            if (++player->worm_turn > (player->worms.size() - 1)) {
                player->worm_turn = 0;
            }
        }
    }

    if (not(++turn_number % 3)) {
        battlefield.createCrate();
    }

    return TurnReset::TIMER_RESET;
}

const ActualTurn TurnHandler::updateTurn(const std::chrono::duration<float>& elapsed) {
    auto reset_timer = TurnReset::NOT_RESET;

    if (not infinite_turn_cheat_activated) {
        reset_timer = this->need_to_update(players.size(), elapsed);
    }

    auto it = players.begin();
    std::advance(it, player_turn);
    uint8_t player_id = it->first;
    // if worm_turn > worms.sz
    auto worm_it = it->second->worms.begin();
    advance(worm_it, it->second->worm_turn);
    worm_turn_id = worm_it->first;


    switch (reset_timer) {

        case TurnReset::TIMER_RESET: {
            current_player_worms_quantity = it->second->worms.size();
            worm_handler.updateTurnWorm(player_id, worm_turn_id);
            battlefield.newWindForce(no_wind_cheat_activated);
            broadcaster.broadcast_turn(player_turn);
            break;
        }

        case TurnReset::WAIT_TURN_END: {
            broadcaster.broadcast_turn(player_turn, BLOCK_PLAYERS_INPUT);
            break;
        }

        default:  // NO RESET
            break;
    }

    return ActualTurn(player_id, it->second->worm_turn);
}

const bool& TurnHandler::player_used_stop_action() { return player_stop_action; }

void TurnHandler::use_stop_action() {
    if (infinite_turn_cheat_activated) {
        return;
    }

    player_stop_action = true;

    elapsed_time = std::chrono::duration<float>(0);
}

void TurnHandler::activateInfiniteTurn() {
    infinite_turn_cheat_activated = !infinite_turn_cheat_activated;
}

void TurnHandler::activateNoWind() {
    no_wind_cheat_activated = !no_wind_cheat_activated;
    battlefield.newWindForce(no_wind_cheat_activated);
}

void TurnHandler::supplyRun() {
    for (size_t i = 0; i < 7; i++) {
        battlefield.createCrate();
    }
}
