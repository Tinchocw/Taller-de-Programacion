#ifndef WORMS2D_BAR_H
#define WORMS2D_BAR_H

#include "../common/const.h"
#include "box2d/box2d.h"

#include "entity.h"

#define BAR_WIDTH_SHORT Config::yamlNode["bar_width_short"].as<float>()
#define BAR_WIDTH_LONG Config::yamlNode["bar_width_long"].as<float>()
#define BAR_HEIGHT Config::yamlNode["bar_height"].as<float>()

class Battlefield;
class LevelHolder;

class Bar: public Entity {
private:
    float width;
    float angle;
    b2Vec2 get_bar_position();
    TerrainActors get_bar_type() const;

    [[nodiscard]] float get_bar_width() const;
    [[nodiscard]] float get_bar_height() const;

public:
    explicit Bar(Battlefield& battlefield, const float& x, const float& y,
                 const float& inclination_angle, const bool& is_long);


    /*
        @brief Según la dirección de la normal de la colisión entre una entidad y la barra
               genera que la entidad se detenga en caso cumplir las condiciones solicitadas
     */
    void collision_reaction(const b2Vec2& normal) override;

    void applyWindResistance(const float& wind_force) override;

    friend class LevelHolder;
};


#endif  // WORMS2D_BAR_H
