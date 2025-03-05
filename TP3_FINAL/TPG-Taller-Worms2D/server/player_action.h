#ifndef COMMAND_H
#define COMMAND_H

#include <memory>
#include <string>
#include <vector>

#include <stdint.h>

#include "../common/States.h"
#include "../common/const.h"
#include "../common/queue.h"

class WormHandler;
class TurnHandler;
class GameBrowser;

namespace ServerSide {
class Protocol;
}

// ----------------------- PLAYER ACTIONS INTERFACE ----------------------

class PlayerAction {
public:
    const uint8_t id;

    explicit PlayerAction(const uint8_t& id): id(id) {}

    /*
        @param worm_handler:
        @param turn_id:
        @param turn_handler:

        @brief Da la interfaz para ejecutar el comando
    */
    virtual void execute(WormHandler& worm_handler, const uint8_t& turn_id,
                         TurnHandler& turn_handler) = 0;

    virtual ~PlayerAction() = default;
};


// ----------------------- START MOVING ----------------------

class StartMoving: public PlayerAction {
private:
    Direction direction;

public:

    /*
        @param protocol: Encargado de recibir la información del cliente
        @param id: id del cliente

        @brief LLama al constructor de PlayerAction, y recibe a traves del protocolo la direccion a moverse
     */
    explicit StartMoving(ServerSide::Protocol& protocol, const uint8_t& id);

    /*
        @brief Delega al servidor el movimiento del gusano
     */
    void execute(WormHandler& worm_handler, const uint8_t& turn_id,
                 TurnHandler& turn_handler) override;

    ~StartMoving() override = default;
};

// ----------------------- STOP MOVING ----------------------

class StopMoving: public PlayerAction {
public:

    /*
        @param id: id del player

        @brief LLama al constructor de PlayerAction, y recibe a través del protocolo la dirección a moverse
     */
    explicit StopMoving(const uint8_t& id);

    /*
        @brief Delega al servidor el movimiento del gusano
     */
    void execute(WormHandler& worm_handler, const uint8_t& turn_id,
                 TurnHandler& turn_handler) override;

    ~StopMoving() override = default;
};

// ----------------------- JUMP ----------------------

class Jump: public PlayerAction {
private:
    JumpDir direction;

public:
    /*
        @param protocol: Encargado de recibir la información del cliente
        @param id: id del player

        @brief LLama al constructor de PlayerAction, y recibe a través del protocolo la dirección a donde saltar
     */
    explicit Jump(ServerSide::Protocol& protocol, const uint8_t& id);

    /*
        @brief Delega al servidor el salto del gusano
     */
    void execute(WormHandler& worm_handler, const uint8_t& turn_id,
                 TurnHandler& turn_handler) override;

    ~Jump() override = default;
};

// ---------------------- ADSAngle -----------------------

class ADSAngle: public PlayerAction {
private:
    ADSAngleDir direction;

public:

    /*
        @param protocol: Encargado de recibir la información del cliente
        @param id: id del player

        @brief LLama al constructor de PlayerAction, y recibe a través del protocolo la dirección a apuntar
     */
    explicit ADSAngle(ServerSide::Protocol& protocol, const uint8_t& id);

    /*
        @brief Delega al servidor el apuntado del arma
     */
    void execute(WormHandler& worm_handler, const uint8_t& turn_id,
                 TurnHandler& turn_handler) override;

    ~ADSAngle() override = default;
};

// -------------------- STOP ADS --------------------

class StopADSAngle: public PlayerAction {
public:

    /*
        @param id: id del player

        @brief LLama al constructor de PlayerAction
     */
    explicit StopADSAngle(const uint8_t& id);

    /*
        @brief Delega al servidor el stop del apuntado del arma
     */
    void execute(WormHandler& worm_handler, const uint8_t& turn_id,
                 TurnHandler& turn_handler) override;

    ~StopADSAngle() override = default;
};

// ------------------------- FIRE POWER -----------------

class FirePower: public PlayerAction {
public:

    /*
        @param id: id del player

        @brief LLama al constructor de PlayerAction
     */
    explicit FirePower(const uint8_t& id);


    /*
        @brief Delega al servidor cargado el comienzo del cargado del arma
     */
    void execute(WormHandler& worm_handler, const uint8_t& turn_id,
                 TurnHandler& turn_handler) override;

    ~FirePower() = default;
};

// ----------------------- SHOOT --------------------

class Shoot: public PlayerAction {
public:

    /*
        @param id: id del player

        @brief LLama al constructor de PlayerAction
     */
    explicit Shoot(const uint8_t& id);

    /*
        @brief Delega al servidor el disparo del arma actual
     */
    void execute(WormHandler& worm_handler, const uint8_t& turn_id,
                 TurnHandler& turn_handler) override;

    ~Shoot() = default;
};

// -------------------- DELAY ------------------

class Delay: public PlayerAction {
private:
    DelayAmount amount;

public:

    /*
        @param protocol: Encargado de recibir la información del cliente
        @param id: id del player

        @brief LLama al constructor de PlayerAction, y recibe a través del protocolo la cantidad de delay
     */
    explicit Delay(ServerSide::Protocol& protocol, const uint8_t& id);

    /*
        @brief Delega al servidor el seteado del delay proyectil a lanzar
     */
    void execute(WormHandler& worm_handler, const uint8_t& turn_id,
                 TurnHandler& turn_handler) override;

    ~Delay() = default;
};

// ---------------------- USE CLICKABLE ---------------------------

class UseClickable: public PlayerAction {
private:
    b2Vec2 position;

public:

    /*
        @param protocol: Encargado de recibir la información del cliente
        @param id: id del player

        @brief LLama al constructor de PlayerAction, y recibe a través del protocolo la posición clickeada
     */
    explicit UseClickable(ServerSide::Protocol& protocol, const uint8_t& id);

    /*
        @brief Delega al servidor el cambio de posición del worm
     */
    void execute(WormHandler& worm_handler, const uint8_t& turn_id,
                 TurnHandler& turn_handler) override;

    ~UseClickable() = default;
};

// ------------------- CHANGE GADGET ---------------------------

class ChangeGadget: public PlayerAction {
private:
    WeaponsAndTools gadget;

public:

    /*
        @param protocol: Encargado de recibir la información del cliente
        @param id: id del player

        @brief LLama al constructor de PlayerAction, y recibe a través del protocolo al arma al cual quiere cambiar
     */
    explicit ChangeGadget(ServerSide::Protocol& protocol, const uint8_t& id);

    /*
        @brief Delega al servidor el cambio de arma de los worms
     */
    void execute(WormHandler& worm_handler, const uint8_t& turn_id,
                 TurnHandler& turn_handler) override;

    ~ChangeGadget() = default;
};

class WW3Cheat: public PlayerAction {
public:

    /*
        @param id: id del player

        @brief LLama al constructor de PlayerAction
     */
    explicit WW3Cheat(const uint8_t& id);

    /*
        @brief Delega al servidor la ejecución del cheat de WW3
     */
    void execute(WormHandler& worm_handler, const uint8_t& turn_id,
                 TurnHandler& turn_handler) override;

    ~WW3Cheat() = default;
};

class NoWindCheat: public PlayerAction {
public:

    /*
        @param id: id del player

        @brief LLama al constructor de PlayerAction
     */
    explicit NoWindCheat(const uint8_t& id);

    /*
        @brief Delega al servidor la ejecución del cheat de NoWind
     */
    void execute(WormHandler& worm_handler, const uint8_t& turn_id,
                 TurnHandler& turn_handler) override;

    ~NoWindCheat() = default;
};

class InfiniteAmmoCheat: public PlayerAction {
public:

    /*
        @param id: id del player

        @brief LLama al constructor de PlayerAction
     */
    explicit InfiniteAmmoCheat(const uint8_t& id);

    /*
        @brief Delega al servidor la ejecución del cheat de InfiniteAmmo
     */
    void execute(WormHandler& worm_handler, const uint8_t& turn_id,
                 TurnHandler& turn_handler) override;

    ~InfiniteAmmoCheat() = default;
};

class SupplyRunCheat: public PlayerAction {
public:

    /*
        @param id: id del player

        @brief LLama al constructor de PlayerAction
     */
    explicit SupplyRunCheat(const uint8_t& id);

    /*
        @brief Delega al servidor la ejecución del cheat de SupplyRun
     */
    void execute(WormHandler& worm_handler, const uint8_t& turn_id,
                 TurnHandler& turn_handler) override;

    ~SupplyRunCheat() = default;
};

class ImmortalWorm: public PlayerAction {
public:

    /*
        @param id: id del player

        @brief LLama al constructor de PlayerAction
     */
    explicit ImmortalWorm(const uint8_t& id);

    /*
        @brief Delega al servidor la ejecución del cheat de ImmortalWorm
     */
    void execute(WormHandler& worm_handler, const uint8_t& turn_id,
                 TurnHandler& turn_handler) override;

    ~ImmortalWorm() = default;
};

class MultipleJumpCheat: public PlayerAction {
public:

    /*
        @param id: id del player

        @brief LLama al constructor de PlayerAction
     */
    explicit MultipleJumpCheat(const uint8_t& id);

    /*
        @brief Delega al servidor la ejecución del cheat de MultipleJump
     */
    void execute(WormHandler& worm_handler, const uint8_t& turn_id,
                 TurnHandler& turn_handler) override;

    ~MultipleJumpCheat() = default;
};

class InfiniteTurnCheat: public PlayerAction {
public:

    /*
        @param id: id del player

        @brief LLama al constructor de PlayerAction
     */
    explicit InfiniteTurnCheat(const uint8_t& id);

    /*
       @brief Delega al servidor la ejecución del cheat de MultipleJump
    */
    void execute(WormHandler& worm_handler, const uint8_t& turn_id,
                 TurnHandler& turn_handler) override;

    ~InfiniteTurnCheat() = default;
};

class Everyone1HPCheat: public PlayerAction {
public:
    /*
        @param id: id del player

        @brief LLama al constructor de PlayerAction
     */
    explicit Everyone1HPCheat(const uint8_t& id);

    /*
       @brief Delega al servidor la ejecución del cheat de MultipleJump
    */
    void execute(WormHandler& worm_handler, const uint8_t& turn_id,
                 TurnHandler& turn_handler) override;

    ~Everyone1HPCheat() = default;
};

// ------------------------ LOBBY ACTIONS -----------------------

class LobbyAction {
public:
    /*
        Constructor default
    */
    LobbyAction() = default;
    /*
        Ejecuta el comando
    */
    virtual void execute() = 0;

    virtual ~LobbyAction() = default;
};

// ----------------------- JOIN ----------------------

class Join: public LobbyAction {
private:
    GameBrowser& gb;
    int16_t& game_id;
    const uint8_t& id;
    Queue<std::shared_ptr<States>>& state_queue;

public:

    /*
        @param id: id del player
        @param gb:
        @param id_to_join:
        @param state_queue:

        @brief LLama al constructor de LobbyAction, y
     */
    explicit Join(GameBrowser& gb, int16_t& id_to_join, const uint8_t& id,
                  Queue<std::shared_ptr<States>>& state_queue);

    /*
       @brief Delega al servidor la ejecución de joinear un jugador a una partida
    */
    void execute() override;

    ~Join() override = default;
};

// ----------------------- CREATE ----------------------

class Create: public Join {
private:
    std::string desc;
    std::string map;

public:

    /*
        @param id: id del player
        @param gb:
        @param id_to_join:
        @param state_queue:

        @brief LLama al constructor de LobbyAction, y
     */
    explicit Create(GameBrowser& gb, int16_t& id_to_create, const uint8_t& id,
                    Queue<std::shared_ptr<States>>& state_queue, ServerSide::Protocol& protocol);

    ~Create() override = default;
};

// ----------------------- READY ------------------------

class Ready: public LobbyAction {
private:
    GameBrowser& gb;
    const uint8_t id;
    const uint8_t id_game;

public:

    /*
        @param id: id del player
        @param uint8_t:
        @param id_game:

        @brief LLama al constructor de LobbyAction, y
     */
    explicit Ready(GameBrowser& gb, const uint8_t& id, const int16_t& id_game):
            gb(gb), id(id), id_game(id_game) {}

    /*
       @brief Delega al servidor la ejecución de setear en listo a un jugador
    */
    void execute() override;

    ~Ready() = default;
};

class ShowGames: public LobbyAction {
private:
    std::vector<std::shared_ptr<GameInfoL>> info;
    GameBrowser& gb;
    Queue<std::shared_ptr<States>>& state_queue;

public:

    /*
        @param gb:
        @param stateQ:

        @brief LLama al constructor de LobbyAction, y
     */
    explicit ShowGames(GameBrowser& gb, Queue<std::shared_ptr<States>>& stateQ):
            gb(gb), state_queue(stateQ) {}

    /*
       @brief Delega al servidor la ejecución de mostrar las partidas habilitadas
    */
    void execute() override;

    ~ShowGames() = default;
};

class ExitGame: public LobbyAction {
private:
    GameBrowser& gb;
    const uint8_t& player_id;
    const int16_t& game_id;

public:
    explicit ExitGame(GameBrowser& gb, const uint8_t& player_id, const int16_t& game_id):
            gb(gb), player_id(player_id), game_id(game_id) {}

    /*
       @brief Delega al servidor la ejecución dejar una partida
    */
    void execute() override;

    ~ExitGame() = default;
};

// ----------------------- NULL_COMMAND ----------------------

class NullCommand: public PlayerAction, public LobbyAction {
public:
    inline NullCommand(): PlayerAction(0) {}

    // Comportamiento nulo
    void execute(WormHandler& worm_handler, const uint8_t& turn_id,
                 TurnHandler& turn_handler) override;

    // Comportamiento nulo
    void execute() override;

    // No hay que liberar nada que no este en stack, destructor default es suficiente.
    ~NullCommand() override = default;
};


#endif
