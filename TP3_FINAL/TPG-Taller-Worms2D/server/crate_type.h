#ifndef CRATE_TYPE_H
#define CRATE_TYPE_H

#include <box2d/box2d.h>

#define MIN_HEALTH_FIRST_AID Config::yamlNode["min_health_first_aid"].as<int>()
#define MAX_HEALTH_FIRST_AID Config::yamlNode["max_health_first_aid"].as<int>()

#define MIN_AMMO_AMMO_BOX Config::yamlNode["min_ammo_ammo_box"].as<int>()
#define MAX_AMMO_AMMO_BOX Config::yamlNode["max_ammo_ammo_box"].as<int>()

#define MIN_DAMAGE_TRAP Config::yamlNode["min_damage_trap"].as<int>()
#define MAX_DAMAGE_TRAP Config::yamlNode["max_damage_trap"].as<int>()

#define CRATE_LENGTH Config::yamlNode["crate_length"].as<float>()

#define MIN_X_CRATE_SPAWN Config::yamlNode["min_x_crate_spawn"].as<float>()
#define MAX_X_CRATE_SPAWN Config::yamlNode["max_x_crate_spawn"].as<float>()
#define Y_CRATE_SPAWN Config::yamlNode["y_crate_spawn"].as<float>()

class Battlefield;

class CrateType {
public:
    virtual void collision_reaction(b2Body*& crate_body, Battlefield& battlefield,
                                    bool& was_opened) = 0;
};

class FirstAid: public CrateType {
private:
    uint8_t health_quantity;

public:
    FirstAid();


    /*
        @param create_body: Cuerpo de la caja
        @param battlefield: Clase encargada de manipular la caja
        @param was_opened: Verdadero si la caja esta abierta, falso en caso contrario

        @brief En el que el parámetro was_opened sea verdadero se le aplica una modificación de vida positiva a las entidades cercanas
     */
    void collision_reaction(b2Body*& crate_body, Battlefield& battlefield,
                            bool& was_opened) override;
};

class AmmoBox: public CrateType {
private:
    uint8_t ammo_quantity;

public:
    AmmoBox();


    /*
        @param create_body: Cuerpo de la caja
        @param battlefield: Clase encargada de manipular la caja
        @param was_opened: Verdadero si la caja esta abierta, falso en caso contrario

        @brief En el que el parámetro was_opened sea verdadero se realiza un aumento de de munición a la entidad que colisiono con la caja
     */
    void collision_reaction(b2Body*& crate_body, Battlefield& battlefield,
                            bool& was_opened) override;
};

class Trap: public CrateType {
private:
    uint8_t epicenter_dmg;

public:
    Trap();

    /*
        @param create_body: Cuerpo de la caja
        @param battlefield: Clase encargada de manipular la caja
        @param was_opened: Verdadero si la caja esta abierta, falso en caso contrario

        @brief En el que el parámetro was_opened sea verdadero se ejecuta una explosión que disminuye la vida de las entidades cercanas
     */
    void collision_reaction(b2Body*& crate_body, Battlefield& battlefield,
                            bool& was_opened) override;
};

#endif
