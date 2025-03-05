#ifndef WORM_H
#define WORM_H

#include <memory>

#include <box2d/box2d.h>

#include "../common/config.h"
#include "../common/const.h"

#include "entity.h"

#define INITIAL_LIFE Config::yamlNode["initial_life"].as<int>()

#define TICK_RATE Config::yamlNode["tick_rate"].as<int>()

#define RADIUS Config::yamlNode["worm_radius"].as<float>()

#define ARM_LENGHT Config::yamlNode["arm_length"].as<float>()

#define POWER_RAISE Config::yamlNode["power_raise"].as<float>()
#define MAX_POWER Config::yamlNode["max_power"].as<int>()
#define ANGLE_VARIATION (b2_pi / 64)


#define INCLINATION_MAX (b2_pi / 2)
#define INCLINATION_MIN (-b2_pi / 2)

#define MIN_SQUARED_VELOCITY Config::yamlNode["min_squared_velocity"].as<float>()
#define MIN_Y_VELOCITY Config::yamlNode["min_y_velocity"].as<int>()
#define MIN_X_VELOCITY Config::yamlNode["max_y_velocity"].as<int>()

#define REFRESH_WALK Config::yamlNode["refresh_walk"].as<int>()

#define MIN_FALLING_DAMAGE_HEIGHT Config::yamlNode["min_falling_damage"].as<float>()
#define MAX_FALLING_DAMAGE Config::yamlNode["max_falling_damage"].as<float>()
#define FALL_DMG_AMP Config::yamlNode["fall_damage_amplification"].as<int>()

class Gadget;
class Projectile;
class TurnHandler;

class Worm: public Entity {
private:
    float life;

    bool facing_right;
    bool is_walking;
    bool is_jumping;
    bool is_backflipping;
    bool falling;
    bool using_tool;
    bool refresh_falling_pos;

    bool aiming;
    float aim_inclination_degrees;  // Radianes
    ADSAngleDir aim_direction;

    bool charging_shoot;
    float weapon_power;

    DelayAmount weapon_delay;
    b2Vec2 clicked_position;

    std::unique_ptr<Gadget>*& selected_weapon;
    WeaponsAndTools& weapon_type;

    bool was_damaged;

    float pos_y_before_falling;
    const bool& allow_multiple_jump;
    const bool& immortal_worms;

    bool drown;
    uint8_t team;


    /*
        @brief Retorna 1 en caso de estar mirando hacia la derecha, -1 en caso contrario
    */
    int facing_factor();

public:
    explicit Worm(Battlefield& battlefield, std::unique_ptr<Gadget>*& selected_weapon,
                  WeaponsAndTools& type, const bool& allow_multiple_jump,
                  const bool& immortal_worms, const b2Vec2& position, const uint8_t& team);

    /*
        @brief Aplica un impulso al body en la dirección en la que está orientado
    */
    void move();
    /*
       @brief Setea la velocidad del body en x en 0
    */
    void stop();
    /*
        @param direction: Dirección en la cual se realiza el salto
        @brief Aplica un impulso en el eje x e y según en la dirección en la que está orientado el body
    */
    void jump(const JumpDir& direction);
    /*
        @brief Setea en falso todos los atributos relacionados al movimiento de la del worm
    */
    void stop_all();

    void reloadAmmo(const uint8_t& ammo) override;

    /*
        @brief Retorna la posición de salida del proyectil a disparar.
    */
    b2Vec2 set_bullet_direction();

    /*
        @brief Retorna la fuerza en ambas componentes del proyectil a ser disparado
    */
    b2Vec2 set_bullet_power();

    /*
        @brief Retorna las componentes del ángulo en el cual se está apuntando
    */
    b2Vec2 set_bullet_angle();

    /*
        @param delay: Tiempo de retraso para la explosión de una herramienta
        @brief Modifica el tiempo de explosión del proyectil a utilizar
    */
    void change_bullet_explosion_delay(const DelayAmount& delay);

    /*
        @param new_position: Nueva posición clickeada
        @brief Modifica clicked_position según la posición recibida por parámetro
    */
    void change_clicked_position(const b2Vec2& new_position);

    /*
        @brief Según valor del atributo aim_direction modifica la dirección de apuntado del arma en uso
    */
    void change_aim_direction();

    /*
        @brief En el caso de no superar la potencía maxima, aumenta de forma constante a la fuerza de disparo del arma en uso
    */
    void change_fire_power(TurnHandler& turn_handler);

    /*
        @brief Ejecuta el disparo del arma en uso
    */
    void shoot(TurnHandler& turn_handler);

    void stop_falling() override;
    void start_falling() override;


    void recibe_life_modification(const float& life_variation) override;

    /*
        @brief Aplica un impulso al elemento pasado por parámetro
     */
    void use_chargeable_weapon(const std::shared_ptr<Projectile>& projectile);

    /*
        @brief Aplica un impulso nulo al elemento pasado por parámetro
    */
    void use_positional_weapon(const std::shared_ptr<Projectile>& throwable);

    /*
        @brief Retorna la posición inicial del proyectil
     */
    b2Vec2 set_projectile_inplace();

    /*
        @brief Modifica la posición del body según el atributo clicked_position
    */
    void change_position();

    /*
        @brief Retorna la posición que fue clickeada
    */
    b2Vec2 clicked_position_();

    /*
        @brief Retorna el delay del proyectil a utilizar
     */
    DelayAmount grenade_explosion_delay();

    bool is_dead() override;
    void collision_reaction(const b2Vec2& normal) override;

    void open_crate(bool& open) override;

    void applyWindResistance(const float& wind_force) override;

    /*
        @brief Retorna la posición del worm
    */
    b2Vec2 getPosition();

    /*
        @brief Retorna la distancia entre la posición del centro de masa del worm y del cuerpo pasado por parámetro
    */
    float distance_to_body(b2Body* body_);

    /*
        @brief Retorna true en caso de estar orientado hacia la derecha, falso en caso contrario
    */
    bool is_facing_right();

    /*
        @brief Setea en verdadero el uso de una herramienta
     */
    void use_tool();

    virtual ~Worm() = default;

    friend class Player;
    friend class BroadCaster;
    friend class InfoParser;
    friend class WormHandler;
    friend class TurnHandler;

    Worm(Worm&& o);
};

#endif
