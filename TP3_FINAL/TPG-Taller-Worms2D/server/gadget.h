#ifndef WORMS2D_GADGET_H
#define WORMS2D_GADGET_H

#include "../common/const.h"
#include "box2d/b2_math.h"

#include "proyectile.h"


#define BAZOOKA_AMMO Config::yamlNode["bazooka_ammo"].as<int>()
#define GREEN_GRENADE_AMMO Config::yamlNode["green_grenade_ammo"].as<int>()
#define BANANA_AMMO Config::yamlNode["banana_ammo"].as<int>()
#define DYNAMITE_AMMO Config::yamlNode["dynamite_ammo"].as<int>()
#define MORTAR_AMMO Config::yamlNode["mortar_ammo"].as<int>()
#define RED_GRENADE_AMMO Config::yamlNode["red_grenade_ammo"].as<int>()
#define HOLY_GRENADE_AMMO Config::yamlNode["holy_grenade_ammo"].as<int>()


#define TELEPORT_AMMO Config::yamlNode["teleport_ammo"].as<int>()
#define AIRSTRIKE_AMMO Config::yamlNode["air_strike_ammo"].as<int>()
#define BASEBALL_BAT_AMMO Config::yamlNode["baseball_bat_ammo"].as<int>()

#define AIRSTRIKE_ROCKETS Config::yamlNode["air_strike_rockets"].as<int>()
#define AIRSTRIKE_ROCKET_SEPARATION Config::yamlNode["air_strike_rocket_separation"].as<float>()
#define AIRSTRIKE_ROCKET_X_DEVIATION Config::yamlNode["air_strike_rocket_deviation_x"].as<float>()
#define AIRSTRIKE_ROCKET_Y_POSITION Config::yamlNode["air_strike_rocket_y_position"].as<int>()

#define BAT_DAMAGE Config::yamlNode["baseball_bat_damage"].as<int>()
#define BAT_POWER_FACTOR Config::yamlNode["baseball_bat_factor"].as<int>()
#define BAT_RANGE Config::yamlNode["baseball_bat_range"].as<int>()


class Worm;
class Game;
class Battlefield;
class TurnHandler;

class Gadget {
protected:
    uint8_t ammo;

public:
    explicit Gadget(const uint8_t& ammo);

    /*
        @param battlefield: Clase que se encarga de almacenar el proyectil disparado y generar las físicas del mismo
        @param worm: Entidad que realiza el disparo
        @param turn_handler: Clase que se encarga de manejar cuando turno del disparo

        @brief Según el tipo de herramienta dispara o ejecuta la acción correspondiente solicitada por el worm
     */
    virtual void shoot(Battlefield& battlefield, Worm& worm, TurnHandler& turn_handler) = 0;
    void infiniteAmmo();
    void addAmmo(const uint8_t& ammo);

    virtual ~Gadget() = default;

    friend class Player;
};

//~~~~~~~~~~~~~~~~~~~ GreenGrenade ~~~~~~~~~~~~~~~~~~~~

class GreenGrenade: public Gadget {
public:
    GreenGrenade();

    /*
        @brief Lanza la granada en una dirección y una fuerza determinada solicitada por el worm
     */
    void shoot(Battlefield& battlefield, Worm& worm, TurnHandler& turn_handler) override;
    ~GreenGrenade() override = default;
};

//~~~~~~~~~~~~~~~~~~~ RedGrenade ~~~~~~~~~~~~~~~~~~~~

class RedGrenade: public Gadget {
public:
    RedGrenade();

    /*
        @brief Lanza la granada en una dirección y una fuerza determinada solicitada por el worm
     */
    void shoot(Battlefield& battlefield, Worm& worm, TurnHandler& turn_handler) override;
    ~RedGrenade() override = default;
};

//~~~~~~~~~~~~~~~~~~~ HolyGrenade ~~~~~~~~~~~~~~~~~~~~

class HolyGrenade: public Gadget {
public:
    HolyGrenade();

    /*
        @brief Lanza la granada en una dirección y una fuerza determinada solicitada por el worm
     */
    void shoot(Battlefield& battlefield, Worm& worm, TurnHandler& turn_handler) override;
    ~HolyGrenade() override = default;
};

//~~~~~~~~~~~~~~~~~~~ Banana ~~~~~~~~~~~~~~~~~~~~

class BananaGrenade: public Gadget {
public:
    BananaGrenade();

    /*
        @brief Lanza la granada en una dirección y una fuerza determinada solicitada por el worm
     */
    void shoot(Battlefield& battlefield, Worm& worm, TurnHandler& turn_handler) override;
    ~BananaGrenade() override = default;
};

//~~~~~~~~~~~~~~~~~~~ DynamiteGrenade ~~~~~~~~~~~~~~~~~~~~

class DynamiteGrenade: public Gadget {
public:
    DynamiteGrenade();

    /*
        @brief Deja la granada en la dirección solicitada por el worm
     */
    void shoot(Battlefield& battlefield, Worm& worm, TurnHandler& turn_handler) override;
    ~DynamiteGrenade() override = default;
};


//~~~~~~~~~~~~~~~~~~~ Bazooka ~~~~~~~~~~~~~~~~~~~~

class Bazooka: public Gadget {
public:
    Bazooka();

    /*
        @brief Lanza el proyectil en una dirección y una fuerza determinada solicitada por el worm
    */
    void shoot(Battlefield& battlefield, Worm& worm, TurnHandler& turn_handler) override;
    ~Bazooka() override = default;
};

//~~~~~~~~~~~~~~~~~~~ Mortar ~~~~~~~~~~~~~~~~~~~~

class Mortar: public Gadget {
public:
    Mortar();

    /*
        @brief Lanza el proyectil en una dirección y una fuerza determinada solicitada por el worm
    */
    void shoot(Battlefield& battlefield, Worm& worm, TurnHandler& turn_handler) override;
    ~Mortar() override = default;
};

//~~~~~~~~~~~~~~~~~~~ Teleport ~~~~~~~~~~~~~~~~~~~~

class Teleport: public Gadget {
public:
    Teleport();

    /*
        @brief Cambia la posición del worm según la posición solicitada por el mismo
    */
    void shoot(Battlefield& battlefield, Worm& worm, TurnHandler& turn_handler) override;
    ~Teleport() override = default;
};

//~~~~~~~~~~~~~~~~~~~ AirStrike ~~~~~~~~~~~~~~~~~~~~

class AirStrike: public Gadget {
public:
    AirStrike();

    /*
        @brief Lanza una cantidad de proyectiles en la zona solicitada por el worm
    */
    void shoot(Battlefield& battlefield, Worm& worm, TurnHandler& turn_handler) override;

    /*
        @brief Lanza una cantidad de proyectiles en la zona solicitada por el parametro destination
    */
    void shootCheat(Battlefield& battlefield, float& destination);
    ~AirStrike() override = default;
};


//~~~~~~~~~~~~~~~~~~~ BaseballBat ~~~~~~~~~~~~~~~~~~~~

class BaseballBat: public Gadget {
private:
    void bat(Battlefield& battlefield, Worm& worm);

    /*
        @param body_: cuerpo a aplicar al impulso
        @param blastCenter: Centro de la explosión
        @param applyPoint: Posición del body sobre el cual se aplica la explosión
        @param blastPower: Fuerza del impulso en el centro de la explosión

        @brief Aplica un impulso al body pasado según su posición respecto del blastCenter y del blastPower
     */
    void applyBlastImpulse(b2Body* body_, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower,
                           b2Vec2 direction);

public:
    BaseballBat();

    /*
        @brief Realiza la acción de bateo aplicando un impulso a entidades cercanas
    */
    void shoot(Battlefield& battlefield, Worm& worm, TurnHandler& turn_handler) override;

    ~BaseballBat() override = default;
};

#endif  // WORMS2D_GADGET_H
