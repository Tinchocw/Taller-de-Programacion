#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <memory>
#include <vector>

#include <box2d/box2d.h>

#include "../common/const.h"

#include "entity.h"
#include "gadget.h"
#include "worm.h"

class Gadget;
class Battlefield;
class Projectile;
class BroadCaster;

class Player {
private:
    bool ready;
    bool is_playing;
    std::map<WeaponsAndTools, std::unique_ptr<Gadget>> weapons;
    std::unique_ptr<Gadget>* selected_weapon;
    WeaponsAndTools selected_gadget_type;
    uint8_t worm_turn;

    std::map<uint8_t, std::shared_ptr<Worm>> worms;

    bool allow_multiple_jump;
    bool immortal_worms;

    /*
        @param battlefield: Referencia al battlefield del juego
        @param worms_quantity: Cantidad de worms a spawnear
        @param worms_counter: Contador de ID de worms
        @param spawn_points: Vector con las posiciones iniciales de los worms

        @brief Setea la posición inicial de todos los worms del jugador
     */
    void spawnWorms(Battlefield& battlefield, const uint8_t& worms_quantity, uint8_t& worm_counter,
                    const uint8_t& id, std::vector<b2Vec2>& spawn_points);

public:
    /*
        @brief Crea al jugador con todas sus armas y seta a la bazooka como arma inicial
     */
    Player();

    /*
        @brief Calcula la vida promedio de todos los worms que estan jugando
     */
    uint8_t calcAvgLife();

    /*
        @brief Setea al jugador en ready
     */
    void set_ready();


    /*
        @param new_weapon: Arma seleccionada

        @brief Cambia el arma por la pasada por parámetro
     */
    void change_weapon(const WeaponsAndTools& new_weapon);

    /*
        @brief Retorna la cantidad de munición de todas las armas
     */
    std::unique_ptr<AmmoLeft> getWeaponsAmmo();

    /*
        @brief Setea a todas las armas con munición infinita
     */
    void infiniteAmmo();

    ~Player() = default;

    friend class Game;
    friend class BroadCaster;
    friend class InfoParser;
    friend class WormHandler;
    friend class TurnHandler;

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
    Player(Player&&);
    Player& operator=(Player&&) = delete;
};

#endif  // PLAYER_H
