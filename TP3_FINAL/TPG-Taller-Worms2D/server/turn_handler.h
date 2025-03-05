#ifndef TURN_HANDLER_H
#define TURN_HANDLER_H

#include <chrono>
#include <map>
#include <memory>
#include <utility>

#include <stdint.h>

#define TURN_DURATION_IN_SECONDS Config::yamlNode["turn_duration_in_seconds"].as<int>()
#define POST_TURN_DURATION_IN_SECONDS Config::yamlNode["post_turn_duration_in_seconds"].as<int>()

#define BLOCK_PLAYERS_INPUT true

#include <condition_variable>
// Por alguna razon si no incluyo conditional variable no compila este struct
struct SomeOneWins: public std::runtime_error {
    SomeOneWins(): std::runtime_error("Someone win the game!") {}
};


enum class TurnReset { NOT_RESET = 0, TIMER_RESET = 1, WAIT_TURN_END = 2 };


struct ActualTurn {
public:
    const uint8_t player_id;
    const uint8_t worm_index;

    explicit inline ActualTurn(const uint8_t& player_id, const uint8_t& worm_index):
            player_id(player_id), worm_index(worm_index) {}
};

class Player;
class WormHandler;
class BroadCaster;
class Battlefield;

class TurnHandler {
private:
    BroadCaster& broadcaster;
    WormHandler& worm_handler;
    Battlefield& battlefield;

    uint8_t player_turn;
    uint8_t worm_turn_id;
    std::chrono::duration<float> elapsed_time;

    std::map<uint8_t, std::unique_ptr<Player>>& players;
    uint8_t current_players_quantity;
    uint8_t current_player_worms_quantity;

    bool player_stop_action;

    bool infinite_turn_cheat_activated;
    bool no_wind_cheat_activated;

    unsigned int turn_number;
    /*
        @param players_quantity: Cantidad de jugadores actualmente en la partida
        @param elapsed: Cantidad de tiempo que demoro el anterior game loop

        @brief Valida si se requiere empezar un nuevo turno o debe continuar el mismo worm
    */
    const TurnReset need_to_update(const uint8_t players_quantity,
                                   const std::chrono::duration<float>& elapsed);
    /*
        @param players_quantity: Cantidad de jugadores actualmente en la partida

        @brief Avanza el turno al proximo worm y/o player
    */
    const TurnReset advanceTurn(const uint8_t& players_quantity);

public:
    explicit inline TurnHandler(std::map<uint8_t, std::unique_ptr<Player>>& players,
                                BroadCaster& broadcaster, WormHandler& worm_handler,
                                Battlefield& battlefield):
            broadcaster(broadcaster),
            worm_handler(worm_handler),
            battlefield(battlefield),
            player_turn(0),
            worm_turn_id(0),
            elapsed_time(0),
            players(players),
            current_players_quantity(0),
            current_player_worms_quantity(0),
            player_stop_action(false),
            infinite_turn_cheat_activated(false),
            no_wind_cheat_activated(false),
            turn_number(0) {}
    /*
        @param elapsed: Cantidad de tiempo que demoro el anterior game loop

        @brief Actualiza el turno si se requiere y retorna la id del jugador y worm activos
    */
    const ActualTurn updateTurn(const std::chrono::duration<float>& elapsed);
    /*
        @brief Retorna si el jugador uso una accion la cual activa el post turno
    */
    const bool& player_used_stop_action();
    /*
        @brief Activa el post turno
    */
    void use_stop_action();
    /*
        @brief Switch de cheat de turno infinito al worm actual
    */
    void activateInfiniteTurn();
    /*
        @brief Switch de cheat eliminacion del viento
    */
    void activateNoWind();
    /*
        @brief Cheat de spawn de 7 cajas de provisiones aleatorias
    */
    void supplyRun();
};

#endif
