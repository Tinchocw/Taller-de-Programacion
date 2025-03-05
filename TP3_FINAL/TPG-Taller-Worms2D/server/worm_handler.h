#ifndef WORM_HANDLER_H
#define WORM_HANDLER_H

#include <map>
#include <memory>
#include <mutex>

#include <box2d/b2_math.h>
#include <stdint.h>

#include "../common/const.h"

class Player;
class Worm;
class TurnHandler;

class WormHandler {
private:
    /*
        std::mutex& m; No hace falta este lock, ya que el unico hilo que utiliza estos metodos es el
       gameloop
    */
    std::map<uint8_t, std::unique_ptr<Player>>& players;

    std::shared_ptr<Worm> turn_worm;


public:
    /*
        @param players: Mapa de jugadores
    */
    explicit WormHandler(std::map<uint8_t, std::unique_ptr<Player>>& players);
    /*
        @param id: ID del jugador con el turno actual
        @param worm_id: ID del worm con el turno actual

        @brief Elige el worm con el turno actual
    */
    void updateTurnWorm(const uint8_t& id, const uint8_t& worm_id);
    /*
        @brief Detiene toda velocidad del worm con el turno
    */
    void stop_turn_worm();
    /*
        @brief Retorna la cuenta de cuantos jugadores quedan vivos (o sea is_alive = true)
    */
    const uint8_t players_alive();
    /*
        @brief Limpia el estado de daniado a todos los worms
    */
    void clearDamagedState();
    /*
        @brief Actualiza las fisicas de movimiento al worm si se esta moviendo
    */
    void update_physics();
    /*
        @param turn_handler: Referencia al turn handler

        @brief Actualiza el estado del arma
    */
    void update_weapon(TurnHandler& turn_handler);
    /*
        @param direction: Direccion a la cual moverse
        @param id: ID del jugador

        @brief Mueve al worm que tiene el turno en la direccion indicada
    */
    void player_start_moving(const Direction& direction, const uint8_t& id);
    /*
        @param id: ID del jugador

        @brief Detiene al worm que tiene el turno
    */
    void player_stop_moving(const uint8_t& id);
    /*
        @param direction: Direccion a la cual saltar
        @param id: ID del jugador

        @brief Salta el worm que tiene el turno en la direccion indicada
    */
    void player_jump(const JumpDir& direction, const uint8_t& id);
    /*
        @param direction: Direccion a la cual comenzar a apuntar verticalmente
        @param id: ID del jugador

        @brief Mueve la mira del worm hacia la direccion indicada
    */
    void player_start_aiming(const ADSAngleDir& direction, const uint8_t& id);
    /*
        @param id: ID del jugador

        @brief Detiene el movimiento de la mira del worm
    */
    void player_stop_aiming(const uint8_t& id);
    /*
        @param id: ID del jugador

        @brief Indica al worm que comience a cargar la potencia del arma
    */
    void player_start_charging(const uint8_t& id);
    /*
        @param turn_handler: Referencia al turn handler para comunicar que se uso una stop action
        @param id: ID del jugador

        @brief Dispara el arma del worm
    */
    void player_shoot(const uint8_t& id, TurnHandler& turn_handler);
    /*
        @param position: Posicion del mouse
        @param id: ID del jugador

        @brief Usa el gadget clickeable del worm en la posicion indicada
    */
    void player_use_clickable(b2Vec2 position, const uint8_t& id);
    /*
        @param delay: Delay a ajustar en el gadget
        @param id: ID del jugador

        @brief Modifica el delay del gadget
    */
    void player_set_delay(DelayAmount delay, const uint8_t& id);
    /*
        @param gadget: Gadget el cual el worm debe equipar
        @param id: ID del jugador

        @brief Modifica el gadget equipado del worm que posee el turno
    */
    void player_change_gadget(const WeaponsAndTools& gadget, const uint8_t& id);
    /*
        @brief Comprueba si hay worms muertos, en caso afirmativo los elimina
    */
    void checkDeadWorms();
    /*
        @brief Comprueba si hay worms en cierta altura en Y, en caso afirmativo los "ahoga"
    */
    void check_drown_worms();
    /*
        @brief Comprueba si el worm que esta cayendo esta en una posicion mas alta que la inicial
    */
    void check_falling_worms();
    /*
        @brief Pone a todos los worms a 1 de vida
    */
    void allWorms1HP();
    /*
        @param id: ID del jugador

        @brief Habilita a los worms del jugador de ID id a hacer saltos multiples
    */
    void playerAllowMultipleJump(const uint8_t& id);
    /*
        @param id: ID del jugador

        @brief Habilita a los worms del jugador de ID id a ser inmortales
    */
    void makePlayerWormsImmortal(const uint8_t& id);
    /*
        @param id: ID del jugador

        @brief Habilita a los worms del jugador de ID id a no perder municion
    */
    void playerInfiniteAmmo(const uint8_t& id);
    /*
        @brief Aparece 5 ataques aereos aleatorios
    */
    void WW3Cheat();
    /*
        @brief Comprueba si siguen cayendo algun worm
    */
    const bool allWormsStayStill();
};


#endif
