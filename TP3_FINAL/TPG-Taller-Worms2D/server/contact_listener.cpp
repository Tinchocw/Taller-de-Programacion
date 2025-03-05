#include "contact_listener.h"

#include <iostream>

#include "Player.h"
#include "bar.h"

void Contact_listener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
    b2ContactListener::PreSolve(contact, oldManifold);

    auto contact2 = contact->GetFixtureB()->GetBody()->GetUserData().pointer;
    auto contact1 = contact->GetFixtureA()->GetBody()->GetUserData().pointer;

    auto* dataB = reinterpret_cast<Entity*>(contact2);
    auto* dataA = reinterpret_cast<Entity*>(contact1);


    // Get the world manifold to obtain the collision normal
    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);

    // Get the collision normal
    b2Vec2 normal = worldManifold.normal;

    if (dataA && dataB) {

        dataA->collision_reaction(normal);
        dataB->collision_reaction(normal);
    }


}



void Contact_listener::EndContact(b2Contact *contact) {
    auto contact2 = contact->GetFixtureB()->GetBody()->GetUserData().pointer;
    auto contact1 = contact->GetFixtureA()->GetBody()->GetUserData().pointer;

    auto* dataB = reinterpret_cast<Entity*>(contact2);
    auto* dataA = reinterpret_cast<Entity*>(contact1);

    if (dataA && dataB) {
        dataA->start_falling();
        dataB->start_falling();
    }


}
