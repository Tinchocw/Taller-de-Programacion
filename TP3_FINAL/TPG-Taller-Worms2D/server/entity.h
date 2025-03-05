#ifndef WORMS2D_ENTITY_H
#define WORMS2D_ENTITY_H

#include <memory>

#include <box2d/box2d.h>

class Battlefield;


class Entity {
protected:
    bool dead;
    b2Body* body;
    Battlefield& battlefield;

public:
    explicit Entity(Battlefield& battlefield);

    /*
       @brief Aplica una fuerza en el eje x simulando el viento
    */
    virtual void applyWindResistance(const float& wind_force) = 0;


    /*
        @param Vector de dirección y fuerza del impulso
        @brief Aplica un impulso al body
     */
    virtual void apply_explosion(const b2Vec2& final_impulse);


    /*
         @brief Detiene al cuerpo cuando está cayendo
     */
    virtual void stop_falling();

    /*
         @brief Setea las condiciones para que el cuerpo arranque a caer
     */
    virtual void start_falling();

    /*
        @param life_modification: Valor de aumentar o disminuir la vida
        @brief Modifica la vida de la entidad
    */
    virtual void recibe_life_modification(const float& life_variation);

    /*
        @param ammo: Cantidad de munición a recargar
        @brief Recarga la munición de la entidad
    */
    virtual inline void reloadAmmo(const uint8_t& ammo) {}

    /*
        @brief Retorna true en caso de estar muerto, false en caso contrario
    */
    virtual bool is_dead();

    /*
        @brief Setea el parámetro según la entidad
    */
    virtual inline void open_crate(bool& open) {}

    /*
        @param normal: Vector normal del contacto entre la entidad actual y la la entidad con la cual colisiono
        @brief Reacción a la colisión con otra entidad
     */
    virtual void collision_reaction(const b2Vec2& normal) = 0;

    virtual ~Entity();

    friend class Engine;
};


#endif  // WORMS2D_ENTITY_H
