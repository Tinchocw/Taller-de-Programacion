#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <stdint.h>

#include "engine.h"
#include "level_holder.h"

class Projectile;
class States;
class Player;
class WormHandler;
class Crate;

class Battlefield {
private:
    Engine engine;
    /*
        std::mutex m; No hace falta este lock, ya que el unico hilo que utiliza estos metodos es el
        gameloop
    */
    std::map<uint8_t, std::shared_ptr<Projectile>> projectiles;
    uint8_t projectile_count;

    std::vector<std::shared_ptr<Crate>> crates;
    uint8_t crate_count;

    LevelHolder level_holder;

    /*
        @brief Actualiza los cronómetros de todos los proyectiles
     */
    void updateProjectilesTimer();

    /*
        @brief Ejecuta la acción post explosión de todos los proyectiles
    */
    void post_action_explosion();

    /*
        @brief Ejecuta la acción de ahogamiento de todos los proyectiles
     */
    void update_drown_projectiles();

    /*
        @brief Ejecuta la acción de ahogamiento de todas las cajas
     */
    void update_drown_crates();

public:
    explicit Battlefield(std::string level_selected);

    /*
        @brief Retorna un proyectil
     */
    std::map<uint8_t, std::shared_ptr<Projectile>>& getProjectiles();

    /*
        @param body: Cuerpo a ser destruido
        @brief Destruye el cuerpo pasado por parámetro
     */
    void destroy_body(b2Body*& body);

    /*
        @param body: Cuerpo a ser agregado
        @brief Agrego el cuerpo pasado por parámetro
     */
    b2Body* add_body(b2BodyDef& bodyDef);

    /*
        @brief Agrega la query_AABB al engine
     */
    void add_query_AABB(b2QueryCallback* callback, const b2AABB& aabb);

    /*
        @brief Actualiza las físicas y los atributos de las entidades
     */
    void step();

    /*
        @param no_wind_cheat_activated: Me indica verdadero si  el cheat de 0 viento está activado, falso en caso contrario
        @brief Genera una fuerza simula el viento en el caso del parámetro estar seteado en falso
    */
    void newWindForce(const bool& no_wind_cheat_activated);

    /*
        @param proyectil: Proyectil a ser agregado
        @brief Agrego el proyectil pasado por parámetro
     */
    void add_projectile(std::shared_ptr<Projectile>& proyectile);

    /*
        @brief Remueve del engine todos los cuerpos que estan muertos
     */
    void remove_dead_objects();

    /*
        @brief Crea una nueva caja
     */
    void createCrate();

    /*
        @brief Retorna verdadero en caso de no haber proyectiles, falso en caso contrario
     */
    const bool noProjectiles();

    ~Battlefield() = default;

    friend class InfoParser;
};


#endif
