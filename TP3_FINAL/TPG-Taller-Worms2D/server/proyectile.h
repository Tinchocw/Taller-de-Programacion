#ifndef WORMS2D_PROYECTILE_H
#define WORMS2D_PROYECTILE_H


#include <chrono>
#include <memory>

#include <box2d/b2_body.h>

#include "../common/States.h"
#include "../common/config.h"
#include "../common/const.h"

#include "entity.h"
#include "query_callback.h"

#define BLAST_RADIUS_BAZOOKA Config::yamlNode["blast_radius_bazooka"].as<int>()
#define BLAST_RADIUS_MORTAR Config::yamlNode["blast_radius_mortar"].as<int>()
#define BLAST_RADIUS_MORTAR_FRAGMENT Config::yamlNode["blast_radius_mortar_fragment"].as<int>()
#define BLAST_RADIUS_GREEN_GRENADE Config::yamlNode["blast_radius_green_grenade"].as<int>()
#define BLAST_RADIUS_RED_GRENADE Config::yamlNode["blast_radius_red_grenade"].as<int>()
#define BLAST_RADIUS_BANANA Config::yamlNode["blast_radius_banana"].as<int>()
#define BLAST_RADIUS_DYNAMITE Config::yamlNode["blast_radius_dynamite"].as<int>()
#define BLAST_RADIUS_AIR_STRIKE Config::yamlNode["blast_radius_air_strike"].as<int>()
#define BLAST_RADIUS_HOLY_GRENADE Config::yamlNode["blast_radius_holy_grenade"].as<int>()

#define EPICENTER_DAMAGE_BAZOOKA Config::yamlNode["epicenter_damage_bazooka"].as<int>()
#define EPICENTER_DAMAGE_MORTAR Config::yamlNode["epicenter_damage_mortar"].as<int>()
#define EPICENTER_DAMAGE_MORTAR_FRAGMENT \
    Config::yamlNode["epicenter_damage_mortar_fragment"].as<int>()

#define EPICENTER_DAMAGE_GREEN_GRENADE Config::yamlNode["epicenter_damage_green_grenade"].as<int>()
#define EPICENTER_DAMAGE_RED_GRENADE Config::yamlNode["epicenter_damage_red_grenade"].as<int>()
#define EPICENTER_DAMAGE_BANANA Config::yamlNode["epicenter_damage_banana"].as<int>()
#define EPICENTER_DAMAGE_DYNAMITE Config::yamlNode["epicenter_damage_dynamite"].as<int>()
#define EPICENTER_DAMAGE_AIR_STRIKE Config::yamlNode["epicenter_damage_air_strike"].as<int>()
#define EPICENTER_DAMAGE_HOLY_GRENADE Config::yamlNode["epicenter_damage_holy_grenade"].as<int>()

#define FRAGMENTS_AMOUNT Config::yamlNode["fragments_amount"].as<int>()
#define FRAGMENT_POWER Config::yamlNode["fragment_power"].as<int>()

#define ROCKET_DELAY Config::yamlNode["rocket_delay"].as<float>()


#define DEGTORAD (180 / b2_pi)
#define TIDE_LEVEL Config::yamlNode["tide_level"].as<int>()
#define LEFT_BORDER Config::yamlNode["left_border"].as<int>()
#define RIGHT_BORDER Config::yamlNode["right_border"].as<int>()
#define DROWNING_VELOCITY_FACTOR Config::yamlNode["drowning_projectile_factor"].as<float>()
#define IMPULSE_REDUCTIVE_FACTOR Config::yamlNode["impulse_reductive_factor"].as<float>()


class Battlefield;
class Game;

class Projectile: public Entity {
protected:
    WeaponsAndTools type;
    int blast_radius;
    int epicenter_damage;
    float time_till_detonation;
    std::chrono::time_point<std::chrono::steady_clock> projectile_timer;

    /*
        @param body_: cuerpo a aplicar al impulso
        @param blastCenter: Centro de la explosión
        @param applyPoint: Posición del body sobre el cual se aplica la explosión
        @param blastPower: Fuerza del impulso en el centro de la explosión

        @brief Aplica un impulso al body pasado según su posición respecto del blastCenter y del blastPower
     */
    void applyBlastImpulse(b2Body* body_, const b2Vec2& blastCenter, const b2Vec2& applyPoint,
                           const float& blastPower);

public:
    Projectile(Battlefield& battlefield, const b2Vec2& position, const int& blast_radius,
               const int& epicenter_damage, const WeaponsAndTools& type,
               const float& explosion_delay);

    /*
        @param power: Componentes de la fuerza del impulso

        @brief Setea un impulso según el valor pasado por parámetro
     */
    void set_power(const b2Vec2& power);

    /*
        @param proyectil_id: Identificador del proyectil
        @brief Retorna el estado actual del proyectil
    */
    std::shared_ptr<ProjectileStateG> get_projectile_state(const uint8_t& projectile_id);

    inline void collision_reaction(const b2Vec2& normal) override {}

    inline void applyWindResistance(const float& wind_force) override {}

    /*
        @brief Según que tipo de proyectil se aplica una reacción luego de su explosión
    */
    virtual void second_collision_reaction() = 0;

    /*
        @brief Actualiza el temporizador de la explosión del proyectil
    */
    virtual void updateTimer() = 0;

    /*
        @brief Realiza la explosión del proyectil
     */
    virtual void collide();


    /*
        @brief Modifica los atributos del proyectil en el caso de estar en el agua para simular que se está hundiendo
     */
    void drowning();

    virtual ~Projectile() = default;
    friend class Game;
};


class Rocket: public Projectile {
protected:
    void apply_explosion(const b2Vec2& final_impulse) override;

public:
    Rocket(Battlefield& battlefield, const b2Vec2& position, const int& blast_radius,
           const int& epicenter_damage, const WeaponsAndTools& type);
    void collision_reaction(const b2Vec2& normal) override;
    void applyWindResistance(const float& wind_force) override;
    void updateTimer() override;
    inline void second_collision_reaction() override{};

    ~Rocket() override = default;
};


class BazookaRocket: public Rocket {
public:
    BazookaRocket(Battlefield& battlefield, const b2Vec2& position);
    ~BazookaRocket() override = default;
};

class MortarRocket: public Rocket {
private:
    int fragments;

public:
    MortarRocket(Battlefield& battlefield, const b2Vec2& position);

    /*
        @brief Genera nuevos proyectiles
    */
    void second_collision_reaction() override;
    ~MortarRocket() override = default;
};

class MortarFragment: public Rocket {
private:
public:
    MortarFragment(Battlefield& battlefield, const b2Vec2& position, const b2Vec2& direction);
    inline void applyWindResistance(const float& wind_force) override{};
    ~MortarFragment() override = default;
};

class AirStrikeRocket: public Rocket {
public:
    AirStrikeRocket(Battlefield& battlefield, const b2Vec2& position);
    inline void applyWindResistance(const float& wind_force) override{};
    ~AirStrikeRocket() override = default;
};

class Grenade: public Projectile {
public:
    Grenade(Battlefield& battlefield, const b2Vec2& position, const float& explosion_delay,
            const uint8_t& blast_radius, const uint8_t& epicenter_damage,
            const WeaponsAndTools& type);
    void collision_reaction(const b2Vec2& normal) override;
    void applyWindResistance(const float& wind_force) override;
    void updateTimer() override;
    inline void second_collision_reaction() override{};
    ~Grenade() override = default;
};


class Green: public Grenade {
public:
    Green(Battlefield& battlefield, const b2Vec2& position, const float& explosion_delay);
};

class Red: public Grenade {
private:
    int fragments;

public:
    Red(Battlefield& battlefield, const b2Vec2& position, const float& explosion_delay);

    /*
        @brief Genera nuevos proyectiles
     */
    void second_collision_reaction() override;
};

class Banana: public Grenade {
public:
    Banana(Battlefield& battlefield, const b2Vec2& position, const float& explosion_delay);
};

class Dynamite: public Grenade {
public:
    Dynamite(Battlefield& battlefield, const b2Vec2& position, const float& explosion_delay);
};

class Holy: public Grenade {
public:
    Holy(Battlefield& battlefield, const b2Vec2& position, const float& explosion_delay);
};


#endif  // WORMS2D_PROYECTILE_H
