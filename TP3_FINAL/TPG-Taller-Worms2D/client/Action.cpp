#include "Action.h"

#include "cprotocol.h"

// -------------------------- ACTION ------------------

void Action::send(ClientSide::Protocol& protocol) { protocol.send(&c, sizeof(uint8_t)); }

// -------------------------- NULL ACTION ------------------------
NullAction::NullAction(): Action(Actions::NULL_) {}

void NullAction::send(ClientSide::Protocol& protocol) {}

// ------------------------------- GAME COMMANDS --------------------------

// ################################ MOVEMENT ###############################

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~ START MOVING ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
StartMoving::StartMoving(const Direction& direction):
        Action(Actions::START_MOVING), direction(direction) {}

void StartMoving::send(ClientSide::Protocol& protocol) {
    Action::send(protocol);
    protocol.send(&direction, sizeof(bool));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ STOP MOVING ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

StopMoving::StopMoving(): Action(Actions::STOP_MOVING) {}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ JUMP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Jump::Jump(const JumpDir& dir): Action(Actions::JUMP), direction(dir) {}

void Jump::send(ClientSide::Protocol& protocol) {
    Action::send(protocol);
    protocol.send(&direction, sizeof(bool));
}

// ################################# OFFENSIVE ACTIONS ###############################pragma
// endregion

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ADSAngle ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ADSAngle::ADSAngle(const ADSAngleDir& dir): Action(Actions::ADS_ANGLE), direction(dir) {}

void ADSAngle::send(ClientSide::Protocol& protocol) {
    Action::send(protocol);
    protocol.send(&direction, sizeof(bool));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ STOP ADS ~~~~~~~~~~~~~~~~~~~~~~~~~~

StopADSAngle::StopADSAngle(): Action(Actions::STOP_ADS_ANGLE) {}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ FirePower ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

FirePower::FirePower(): Action(Actions::FIRE_POWER) {}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Shoot ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Shoot::Shoot(): Action(Actions::SHOOT) {}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Use Clickable ~~~~~~~~~~~~~~~~~~~~

void UseClickable::send(ClientSide::Protocol& protocol) {
    Action::send(protocol);
    protocol.sendFloat(x);
    protocol.sendFloat(y);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Delay ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Delay::Delay(const DelayAmount& amount): Action(Actions::DELAY), amount(amount) {}

void Delay::send(ClientSide::Protocol& protocol) {
    Action::send(protocol);
    protocol.send(&amount, sizeof(uint8_t));
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ChangeGadget ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ChangeGadget::ChangeGadget(const WeaponsAndTools& gadget):
        Action(Actions::CHANGE_WEAPON_OR_TOOL), gadget(gadget) {}

void ChangeGadget::send(ClientSide::Protocol& protocol) {
    Action::send(protocol);
    protocol.send(&gadget, sizeof(uint8_t));
}

// ------------------------------- LOBBY COMMANDS --------------------------

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE GAME ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CreateGame::send(ClientSide::Protocol& protocol) {
    Action::send(protocol);
    protocol.sendString64(description);
    protocol.sendString64(map);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ JOIN GAME ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void JoinGame::send(ClientSide::Protocol& protocol) {
    Action::send(protocol);
    protocol.send(&game_id, sizeof(uint8_t));
}
