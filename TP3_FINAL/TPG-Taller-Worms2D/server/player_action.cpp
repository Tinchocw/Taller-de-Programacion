#include "player_action.h"

#include "game_browser.h"
#include "sprotocol.h"
#include "turn_handler.h"
#include "worm_handler.h"

// ----------------------- COMMAND ----------------------

// ----------------------- NULL_COMMAND ----------------------

void NullCommand::execute(WormHandler& worm_handler, const uint8_t& turn_idd,
                          TurnHandler& turn_handler) {}

void NullCommand::execute() {}

// ----------------------- START MOVING ----------------------

StartMoving::StartMoving(ServerSide::Protocol& protocol, const uint8_t& id): PlayerAction(id) {
    protocol.recvDirection(this->direction);
}

void StartMoving::execute(WormHandler& worm_handler, const uint8_t& turn_id,
                          TurnHandler& turn_handler) {
    if (turn_id != this->id) {
        return;
    }

    worm_handler.player_start_moving(direction, id);
}

// ----------------------- STOP MOVING ----------------------

StopMoving::StopMoving(const uint8_t& id): PlayerAction(id) {}

void StopMoving::execute(WormHandler& worm_handler, const uint8_t& turn_id,
                         TurnHandler& turn_handler) {
    if (turn_id != this->id) {
        return;
    }

    worm_handler.player_stop_moving(id);
}


// ----------------------- JUMP ----------------------

Jump::Jump(ServerSide::Protocol& protocol, const uint8_t& id): PlayerAction(id) {
    protocol.recvJumpDir(this->direction);
}

void Jump::execute(WormHandler& worm_handler, const uint8_t& turn_id, TurnHandler& turn_handler) {
    if (turn_id != this->id) {
        return;
    }

    worm_handler.player_jump(direction, id);
}

// ---------------------------- ADSAngle ------------------

ADSAngle::ADSAngle(ServerSide::Protocol& protocol, const uint8_t& id): PlayerAction(id) {
    protocol.recvADSAngleDir(this->direction);
}

void ADSAngle::execute(WormHandler& worm_handler, const uint8_t& turn_id,
                       TurnHandler& turn_handler) {
    if (turn_id != this->id) {
        return;
    }

    if (turn_handler.player_used_stop_action()) {
        return;
    }

    worm_handler.player_start_aiming(direction, id);
}

// -------------------------- STOP ADS ---------------

StopADSAngle::StopADSAngle(const uint8_t& id): PlayerAction(id) {}

void StopADSAngle::execute(WormHandler& worm_handler, const uint8_t& turn_id,
                           TurnHandler& turn_handler) {
    if (turn_id != this->id) {
        return;
    }

    if (turn_handler.player_used_stop_action()) {
        return;
    }

    worm_handler.player_stop_aiming(id);
}

// ---------------------------- FIRE POWER ------------------

FirePower::FirePower(const uint8_t& id): PlayerAction(id) {}

void FirePower::execute(WormHandler& worm_handler, const uint8_t& turn_id,
                        TurnHandler& turn_handler) {
    if (turn_id != this->id) {
        return;
    }

    if (turn_handler.player_used_stop_action()) {
        return;
    }

    worm_handler.player_start_charging(id);
}

// ---------------------------- SHOOT ------------------

Shoot::Shoot(const uint8_t& id): PlayerAction(id) {}

void Shoot::execute(WormHandler& worm_handler, const uint8_t& turn_id, TurnHandler& turn_handler) {
    if (turn_id != this->id) {
        return;
    }

    if (turn_handler.player_used_stop_action()) {
        return;
    }

    worm_handler.player_shoot(id, turn_handler);
}

// ---------------------------- DELAY ------------------

Delay::Delay(ServerSide::Protocol& protocol, const uint8_t& id): PlayerAction(id) {
    protocol.recvDelay(this->amount);
}

void Delay::execute(WormHandler& worm_handler, const uint8_t& turn_id, TurnHandler& turn_handler) {
    if (turn_id != this->id) {
        return;
    }

    worm_handler.player_set_delay(amount, id);

    if (turn_handler.player_used_stop_action()) {
        return;
    }
}

// ---------------------- USE CLICKABLE ---------------------------

UseClickable::UseClickable(ServerSide::Protocol& protocol, const uint8_t& id): PlayerAction(id) {
    protocol.recvPosition(this->position);
}

void UseClickable::execute(WormHandler& worm_handler, const uint8_t& turn_id,
                           TurnHandler& turn_handler) {
    if (turn_id != this->id) {
        return;
    }

    if (turn_handler.player_used_stop_action()) {
        return;
    }

    worm_handler.player_use_clickable(position, id);
    worm_handler.player_shoot(id, turn_handler);

    turn_handler.use_stop_action();
}

// ---------------------------- CHANGE GADGET ------------------

ChangeGadget::ChangeGadget(ServerSide::Protocol& protocol, const uint8_t& id): PlayerAction(id) {
    protocol.recvGadget(this->gadget);
}

void ChangeGadget::execute(WormHandler& worm_handler, const uint8_t& turn_id,
                           TurnHandler& turn_handler) {
    if (turn_id != this->id) {
        return;
    }

    if (turn_handler.player_used_stop_action()) {
        return;
    }

    worm_handler.player_change_gadget(gadget, id);
}

// ----------------------- WW3Cheat ----------------------

WW3Cheat::WW3Cheat(const uint8_t& id): PlayerAction(id) {}

void WW3Cheat::execute(WormHandler& worm_handler, const uint8_t& turn_id,
                       TurnHandler& turn_handler) {
    if (turn_id != this->id) {
        return;
    }

    worm_handler.WW3Cheat();
}

// ----------------------- NoWindCheat ----------------------

NoWindCheat::NoWindCheat(const uint8_t& id): PlayerAction(id) {}

void NoWindCheat::execute(WormHandler& worm_handler, const uint8_t& turn_id,
                          TurnHandler& turn_handler) {
    if (turn_id != this->id) {
        return;
    }

    turn_handler.activateNoWind();
}

// ----------------------- InfiniteAmmoCheat ----------------------

InfiniteAmmoCheat::InfiniteAmmoCheat(const uint8_t& id): PlayerAction(id) {}

void InfiniteAmmoCheat::execute(WormHandler& worm_handler, const uint8_t& turn_id,
                                TurnHandler& turn_handler) {
    if (turn_id != this->id) {
        return;
    }

    worm_handler.playerInfiniteAmmo(turn_id);
}

// ----------------------- SupplyRunCheat ----------------------

SupplyRunCheat::SupplyRunCheat(const uint8_t& id): PlayerAction(id) {}

void SupplyRunCheat::execute(WormHandler& worm_handler, const uint8_t& turn_id,
                             TurnHandler& turn_handler) {
    if (turn_id != this->id) {
        return;
    }

    turn_handler.supplyRun();
}

// ----------------------- ImmortalWorm ----------------------

ImmortalWorm::ImmortalWorm(const uint8_t& id): PlayerAction(id) {}

void ImmortalWorm::execute(WormHandler& worm_handler, const uint8_t& turn_id,
                           TurnHandler& turn_handler) {
    if (turn_id != this->id) {
        return;
    }

    worm_handler.makePlayerWormsImmortal(turn_id);
}

// ----------------------- MultipleJumpCheat ----------------------

MultipleJumpCheat::MultipleJumpCheat(const uint8_t& id): PlayerAction(id) {}

void MultipleJumpCheat::execute(WormHandler& worm_handler, const uint8_t& turn_id,
                                TurnHandler& turn_handler) {
    if (turn_id != this->id) {
        return;
    }

    worm_handler.playerAllowMultipleJump(turn_id);
}

// ----------------------- InfiniteTurnCheat ----------------------

InfiniteTurnCheat::InfiniteTurnCheat(const uint8_t& id): PlayerAction(id) {}

void InfiniteTurnCheat::execute(WormHandler& worm_handler, const uint8_t& turn_id,
                                TurnHandler& turn_handler) {
    if (turn_id != this->id) {
        return;
    }

    turn_handler.activateInfiniteTurn();
}

// ----------------------- Everyone1HPCheat ----------------------

Everyone1HPCheat::Everyone1HPCheat(const uint8_t& id): PlayerAction(id) {}

void Everyone1HPCheat::execute(WormHandler& worm_handler, const uint8_t& turn_id,
                               TurnHandler& turn_handler) {
    if (turn_id != this->id) {
        return;
    }

    worm_handler.allWorms1HP();
}

// ----------------------- JOIN ----------------------

Join::Join(GameBrowser& gb, int16_t& id_to_join, const uint8_t& id,
           Queue<std::shared_ptr<States>>& state_queue):
        gb(gb), game_id(id_to_join), id(id), state_queue(state_queue) {}

void Join::execute() { gb.join_game(game_id, id, state_queue); }

// ----------------------- CREATE ----------------------

Create::Create(GameBrowser& gb, int16_t& id_to_create, const uint8_t& id,
               Queue<std::shared_ptr<States>>& state_queue, ServerSide::Protocol& protocol):
        Join(gb, id_to_create, id, state_queue) {
    protocol.recvString64(desc);
    protocol.recvString64(map);
    gb.create_game(desc, map, id_to_create);
}

// -------------------- SHOW GAMES -----------------

void ShowGames::execute() {
    gb.infoGames(info);
    state_queue.push(std::make_shared<GamesCountL>(info.size()));

    for (const auto& i: info) {
        state_queue.push(i);
    }
}

// -------------------- EXIT GAME ----------------------

void ExitGame::execute() { gb.removeLobbyPlayer(player_id, game_id); }

// ----------------------- READY --------------------

void Ready::execute() { gb.set_player_ready(id, id_game); }
