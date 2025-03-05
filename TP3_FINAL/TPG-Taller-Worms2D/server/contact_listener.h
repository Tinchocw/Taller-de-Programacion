#ifndef WORMS2D_CONTACT_LISTENER_H
#define WORMS2D_CONTACT_LISTENER_H

#include <memory>
#include <vector>

#include <box2d/box2d.h>

#include "entity.h"


class Contact_listener: public b2ContactListener {
public:
    /*
        @brief Resuelve el pre contacto entre dos cuerpos
    */
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
    /*
        @brief Resuelve el post contacto entre dos cuerpos
    */
    void EndContact(b2Contact* contact) override;
    friend class Engine;
};


#endif  // WORMS2D_CONTACT_LISTENER_H
