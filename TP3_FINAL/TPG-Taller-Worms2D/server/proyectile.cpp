#include "proyectile.h"

#include "battlefield.h"

Projectile::Projectile(Battlefield& battlefield, const b2Vec2& position, const int& blast_radius,
                       const int& epicenter_damage, const WeaponsAndTools& type,
                       const float& explosion_delay):
        Entity(battlefield),
        type(type),
        blast_radius(blast_radius),
        epicenter_damage(epicenter_damage),
        time_till_detonation(explosion_delay),
        projectile_timer(std::chrono::steady_clock::now()) {

    b2BodyDef projectile_body;
    projectile_body.type = b2_dynamicBody;
    projectile_body.bullet = true;
    projectile_body.position = b2Vec2(position.x, position.y);
    projectile_body.userData.pointer = reinterpret_cast<uintptr_t>(this);
    body = battlefield.add_body(projectile_body);

    b2CircleShape shape;
    shape.m_radius = 0.25f;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.restitution = 0.2f;

    fixtureDef.filter.groupIndex = -1;  // Para que no colisione con los demás proyectiles

    body->CreateFixture(&fixtureDef);
}


std::shared_ptr<ProjectileStateG> Projectile::get_projectile_state(const uint8_t& projectile_id) {

    float vel_angle = b2Atan2(body->GetLinearVelocity().y, body->GetLinearVelocity().x);

    return std::make_shared<ProjectileStateG>(projectile_id, body->GetPosition().x,
                                              body->GetPosition().y, this->time_till_detonation,
                                              type, dead, vel_angle);
}

void Projectile::set_power(const b2Vec2& power) { body->ApplyLinearImpulseToCenter(power, true); }

void Projectile::collide() {
    if (dead) {
        return;
    }

    Query_callback queryCallback;
    b2AABB aabb;
    aabb.lowerBound = body->GetPosition() - b2Vec2(blast_radius, blast_radius);
    aabb.upperBound = body->GetPosition() + b2Vec2(blast_radius, blast_radius);
    battlefield.add_query_AABB(&queryCallback, aabb);

    // check which of these bodies have their center of mass within the blast radius
    for (int i = 0; i < queryCallback.found_bodies_size(); i++) {
        b2Body* body_ = queryCallback.found_bodie_at(i);
        b2Vec2 bodyCom = body_->GetWorldCenter();

        // ignore bodies outside the blast range
        if ((bodyCom - body->GetWorldCenter()).Length() >= blast_radius)
            continue;

        applyBlastImpulse(body_, body->GetWorldCenter(), bodyCom, epicenter_damage);
    }
}


void Projectile::applyBlastImpulse(b2Body* body_, const b2Vec2& blastCenter,
                                   const b2Vec2& applyPoint, const float& blastPower) {


    b2Vec2 blastDir = applyPoint - blastCenter;
    float distance = blastDir.Normalize();

    // float impulseMag;

    // float invDistance = 1 / distance;
    // impulseMag = blastPower * invDistance;
    // D * (R - o) / R
    float damage = blastPower * (blast_radius - distance) / blast_radius;

    Entity* entity = reinterpret_cast<Entity*>(body_->GetUserData().pointer);

    b2Vec2 final_impulse = damage * IMPULSE_REDUCTIVE_FACTOR * blastDir;
    entity->apply_explosion(final_impulse);
    entity->recibe_life_modification(-damage);
}

void Projectile::drowning() {

    if (body->GetPosition().y <= TIDE_LEVEL) {
        if (body->GetPosition().x < LEFT_BORDER || body->GetPosition().x > RIGHT_BORDER) {
            dead = true;

        } else {
            body->SetLinearVelocity(DROWNING_VELOCITY_FACTOR * body->GetLinearVelocity());
            time_till_detonation -= 0.2f;
        }
    }
}
//~~~~~~~~~~~~~~~~~~~ Rocket ~~~~~~~~~~~~~~~~~~~~

Rocket::Rocket(Battlefield& battlefield, const b2Vec2& position, const int& blast_radius,
               const int& epicenter_damage, const WeaponsAndTools& type):
        Projectile(battlefield, position, blast_radius, epicenter_damage, type, ROCKET_DELAY) {}

void Rocket::collision_reaction(const b2Vec2& normal) {
    if (not dead) {
        collide();
        dead = true;
    }
}

void Rocket::applyWindResistance(const float& wind_force) {
    this->body->ApplyForce(b2Vec2(wind_force, 0), this->body->GetWorldCenter(), true);
}

void Rocket::apply_explosion(const b2Vec2& final_impulse) { Entity::apply_explosion(b2Vec2(0, 0)); }

void Rocket::updateTimer() {
    if (dead) {
        return;
    }

    std::chrono::duration<double> elapsed_seconds =
            std::chrono::steady_clock::now() - projectile_timer;
    projectile_timer = std::chrono::steady_clock::now();
    time_till_detonation -= elapsed_seconds.count();
    if (time_till_detonation <= 0) {
        collide();
        dead = true;
    }
}

//~~~~~~~~~~~~~~~~~~~ Bazooka ~~~~~~~~~~~~~~~~~~~~

BazookaRocket::BazookaRocket(Battlefield& battlefield, const b2Vec2& position):
        Rocket(battlefield, position, BLAST_RADIUS_BAZOOKA, EPICENTER_DAMAGE_BAZOOKA,
               WeaponsAndTools::BAZOOKA) {}


//~~~~~~~~~~~~~~~~~~~ Mortar ~~~~~~~~~~~~~~~~~~~~

MortarRocket::MortarRocket(Battlefield& battlefield, const b2Vec2& position):
        Rocket(battlefield, position, BLAST_RADIUS_MORTAR, EPICENTER_DAMAGE_MORTAR,
               WeaponsAndTools::MORTAR),
        fragments(FRAGMENT_POWER) {}

void MortarRocket::second_collision_reaction() {
    if (not dead) {
        return;
    }

    for (int i = 0; i < fragments; i++) {

        float angle = (i / (float)fragments) * 360 * DEGTORAD;
        b2Vec2 rayDir(sinf(angle), cosf(angle));

        b2Vec2 position;
        position.x = body->GetPosition().x;
        position.y = body->GetPosition().y + 1.0f;

        std::shared_ptr<Projectile> projectile =
                std::make_shared<MortarFragment>(battlefield, position, rayDir);
        battlefield.add_projectile(projectile);
    }
}


//~~~~~~~~~~~~~~~~~~~ MortarFragment ~~~~~~~~~~~~~~~~~~~~

MortarFragment::MortarFragment(Battlefield& battlefield, const b2Vec2& position,
                               const b2Vec2& direction):
        Rocket(battlefield, position, BLAST_RADIUS_MORTAR_FRAGMENT,
               EPICENTER_DAMAGE_MORTAR_FRAGMENT, WeaponsAndTools::MORTAR_FRAGMENT) {

    body->ApplyLinearImpulseToCenter(direction, true);
}

//~~~~~~~~~~~~~~~~~~~ AirStrikeRocket ~~~~~~~~~~~~~~~~~~~~

AirStrikeRocket::AirStrikeRocket(Battlefield& battlefield, const b2Vec2& position):
        Rocket(battlefield, position, BLAST_RADIUS_AIR_STRIKE, EPICENTER_DAMAGE_AIR_STRIKE,
               WeaponsAndTools::AIR_STRIKE) {

    body->GetFixtureList()->SetDensity(40.0f);
}

//~~~~~~~~~~~~~~~~~~~ Grenade ~~~~~~~~~~~~~~~~~~~~

Grenade::Grenade(Battlefield& battlefield, const b2Vec2& position, const float& explosion_delay,
                 const uint8_t& blast_radius, const uint8_t& epicenter_damage,
                 const WeaponsAndTools& type):
        Projectile(battlefield, position, blast_radius, epicenter_damage, type, explosion_delay) {}

void Grenade::collision_reaction(const b2Vec2& normal) {}

void Grenade::updateTimer() {
    if (dead) {
        return;
    }

    std::chrono::duration<double> elapsed_seconds =
            std::chrono::steady_clock::now() - projectile_timer;
    projectile_timer = std::chrono::steady_clock::now();
    time_till_detonation -= elapsed_seconds.count();
    if (time_till_detonation <= 0) {
        collide();
        dead = true;
    }
}

void Grenade::applyWindResistance(const float& wind_force) {}


Green::Green(Battlefield& battlefield, const b2Vec2& position, const float& explosion_delay):
        Grenade(battlefield, position, explosion_delay, BLAST_RADIUS_GREEN_GRENADE,
                EPICENTER_DAMAGE_GREEN_GRENADE, WeaponsAndTools::GREEN_GRENADE) {}

Red::Red(Battlefield& battlefield, const b2Vec2& position, const float& explosion_delay):
        Grenade(battlefield, position, explosion_delay, BLAST_RADIUS_RED_GRENADE,
                EPICENTER_DAMAGE_RED_GRENADE, WeaponsAndTools::RED_GRENADE),
        fragments(FRAGMENTS_AMOUNT) {}

void Red::second_collision_reaction() {
    if (not dead) {
        return;
    }
    for (int i = 0; i < fragments; i++) {

        float angle = (i / (float)fragments) * 360 * DEGTORAD;
        b2Vec2 rayDir(sinf(angle), cosf(angle));

        b2Vec2 position;
        position.x = body->GetPosition().x;
        position.y = body->GetPosition().y + 1.0f;

        std::shared_ptr<Projectile> projectile =
                std::make_shared<MortarFragment>(battlefield, position, rayDir);
        battlefield.add_projectile(projectile);
    }
}

Banana::Banana(Battlefield& battlefield, const b2Vec2& position, const float& explosion_delay):
        Grenade(battlefield, position, explosion_delay, BLAST_RADIUS_BANANA,
                EPICENTER_DAMAGE_BANANA, WeaponsAndTools::BANANA) {
    body->GetFixtureList()->SetRestitution(0.9);
}

Dynamite::Dynamite(Battlefield& battlefield, const b2Vec2& position, const float& explosion_delay):
        Grenade(battlefield, position, explosion_delay, BLAST_RADIUS_DYNAMITE,
                EPICENTER_DAMAGE_DYNAMITE, WeaponsAndTools::DYNAMITE) {}


Holy::Holy(Battlefield& battlefield, const b2Vec2& position, const float& explosion_delay):
        Grenade(battlefield, position, explosion_delay, BLAST_RADIUS_HOLY_GRENADE,
                EPICENTER_DAMAGE_HOLY_GRENADE, WeaponsAndTools::HOLY_GRENADE) {}
