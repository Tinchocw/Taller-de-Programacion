#ifndef CRATE_H
#define CRATE_H

#include <memory>

#include "../common/const.h"

#include "crate_type.h"
#include "entity.h"

class Battlefield;

class Crate: public Entity {
private:
    std::shared_ptr<CrateType> type;
    _CrateType_ _type;
    bool falling;
    bool was_opened;

public:
    const uint8_t crate_id;
    /*
        @brief Genera una caja de tipo random que puede contener municion, vida o explotar
    */
    explicit Crate(Battlefield& battlefield, const uint8_t& id);

    void collision_reaction(const b2Vec2& normal) override;

    void stop_falling() override;

    inline void applyWindResistance(const float& wind_force) override {}

    /*
        @brief Retorna verdadero en caso estar abierta, falso en caso contrario
     */
    const bool wasOpened();

    /*
        @brief Retorna verdadero en caso de haber caído al agua, falso en caso contrario
     */
    const bool wasDrown();

    friend class InfoParser;

    ~Crate();
};

#endif
