#ifndef ACTION_H
#define ACTION_H

#include <string>

#include <stdint.h>

#include "../common/const.h"
namespace ClientSide {
class Protocol;
}

// ------------------------------- ACTION INTERFACE --------------------------

class Action {
public:
    const Actions c;

    explicit Action(const Actions& c): c(c) {}
    /*
        @param protocol: Protocolo del cliente
        @brief Envia los atributos de la accion necesarios para que sea ejecutada en el servidor
    */
    virtual void send(ClientSide::Protocol& protocol);

    virtual ~Action() = default;
};

class NullAction: public Action {
public:
    NullAction();

    void send(ClientSide::Protocol& protocol) override;

    ~NullAction() = default;
};

// ------------------------------- GAME ACTIONS --------------------------

// ################################ MOVEMENT #################################

class StartMoving: public Action {
private:
    const Direction direction;

public:
    explicit StartMoving(const Direction& facing_right);

    void send(ClientSide::Protocol& protocol) override;

    ~StartMoving() override = default;
};

class StopMoving: public Action {
public:
    StopMoving();

    ~StopMoving() override = default;
};

class Jump: public Action {
private:
    const JumpDir direction;

public:
    explicit Jump(const JumpDir& dir);

    void send(ClientSide::Protocol& protocol) override;

    ~Jump() = default;
};

// #################################### OFFENSIVE ACTIONS #################################

class ADSAngle: public Action {
private:
    const ADSAngleDir direction;

public:
    explicit ADSAngle(const ADSAngleDir& dir);

    void send(ClientSide::Protocol& protocol) override;

    ~ADSAngle() = default;
};

class StopADSAngle: public Action {
public:
    StopADSAngle();

    ~StopADSAngle() = default;
};

class FirePower: public Action {
public:
    FirePower();

    ~FirePower() = default;
};

class Shoot: public Action {
public:
    Shoot();

    ~Shoot() = default;
};

class UseClickable: public Action {
private:
    const float x;
    const float y;

public:
    UseClickable(const float& x, const float& y): Action(Actions::USE_CLICKABLE), x(x), y(y) {}

    void send(ClientSide::Protocol& protocol) override;

    ~UseClickable() = default;
};

class Delay: public Action {
private:
    const DelayAmount amount;

public:
    explicit Delay(const DelayAmount& amount);

    void send(ClientSide::Protocol& protocol) override;

    ~Delay() = default;
};

class ChangeGadget: public Action {
private:
    const WeaponsAndTools gadget;

public:
    explicit ChangeGadget(const WeaponsAndTools& gadget);

    void send(ClientSide::Protocol& protocol) override;

    ~ChangeGadget() = default;
};

// ----------------------------------- LOBBY ACTIONS -------------------------

class CreateGame: public Action {
private:
    const std::string description;
    const std::string map;

public:
    CreateGame(const std::string& desc, const std::string& map):
            Action(Actions::CREATE), description(desc), map(map) {}

    void send(ClientSide::Protocol& protocol) override;

    ~CreateGame() override = default;
};

class JoinGame: public Action {
private:
    const uint8_t game_id;

public:
    explicit JoinGame(const uint8_t& game_id): Action(Actions::JOIN), game_id(game_id) {}

    void send(ClientSide::Protocol& protocol) override;

    ~JoinGame() override = default;
};

class ShowGames: public Action {
public:
    ShowGames(): Action(Actions::SHOW_GAMES) {}

    ~ShowGames() = default;
};

class ExitGame: public Action {
public:
    ExitGame(): Action(Actions::EXIT_GAME) {}

    ~ExitGame() = default;
};

class Ready: public Action {
public:
    Ready(): Action(Actions::READY) {}

    ~Ready() = default;
};

class WW3Cheat: public Action {
public:
    WW3Cheat(): Action(Actions::WW3) {}

    ~WW3Cheat() = default;
};

class NoWindCheat: public Action {
public:
    NoWindCheat(): Action(Actions::RANDOM_KILL) {}

    ~NoWindCheat() = default;
};

class InfiniteAmmoCheat: public Action {
public:
    InfiniteAmmoCheat(): Action(Actions::INFINITE_AMMO) {}

    ~InfiniteAmmoCheat() = default;
};

class SupplyRunCheat: public Action {
public:
    SupplyRunCheat(): Action(Actions::MADNESS) {}

    ~SupplyRunCheat() = default;
};

class ImmortalWorm: public Action {
public:
    ImmortalWorm(): Action(Actions::IMMORTAL_WORM) {}

    ~ImmortalWorm() = default;
};

class MultipleJumpCheat: public Action {
public:
    MultipleJumpCheat(): Action(Actions::MULTIPLE_JUMP) {}

    ~MultipleJumpCheat() = default;
};

class InfiniteTurnCheat: public Action {
public:
    InfiniteTurnCheat(): Action(Actions::INFINITE_TURN) {}

    ~InfiniteTurnCheat() = default;
};

class Everyone1HPCheat: public Action {
public:
    Everyone1HPCheat(): Action(Actions::EVERYONE_1HP) {}

    ~Everyone1HPCheat() = default;
};

#endif
