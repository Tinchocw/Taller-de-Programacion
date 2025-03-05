
#ifndef WORMS2D_ENGINE_H
#define WORMS2D_ENGINE_H

#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "../common/config.h"
#include "box2d/box2d.h"

#include "contact_listener.h"

#define X_GRAVITY Config::yamlNode["gravity_x"].as<float>()
#define Y_GRAVITY Config::yamlNode["gravity_y"].as<float>()

#define TIME_STEP Config::yamlNode["time_step"].as<float>()
#define VELOCITY_ITERATIONS Config::yamlNode["velocity_iterations"].as<int>()
#define POSITION_ITERATIONS Config::yamlNode["position_iterations"].as<int>()

#define MIN_WIND Config::yamlNode["min_wind"].as<float>()
#define MAX_WIND Config::yamlNode["max_wind"].as<float>()

class Projectile;

class Engine {
private:
    b2Vec2 gravity;
    std::unique_ptr<b2World> world;
    Contact_listener* listener;

    float wind_force;

    std::uniform_real_distribution<float> random_distribution;
    std::mt19937 rng;

    /*
        @brief Crea todas las clases para necesarias para que funcione en engine
     */
    void create_battlefield();

    /*
        @brief Aplica una fuerza en la componente x que simula en viento a todos los cuerpos del mundo
    */
    void applyWindForce();

public:
    Engine();

    /*
        @param bodyDef: Cuerpo a ser agregado

        @brief Agrega un cuerpo al mundo de Box2d
     */
    b2Body* add_body(b2BodyDef& bodyDef);

    /*
        @param callback: Parámetro necesario para crear la query_AABB
        @param aabb: Parámetro necesario para crear la query_AABB
        @brief Agrega query_AABB al mundo de box2d
     */
    void add_query_AABB(b2QueryCallback* callback, const b2AABB& aabb);

    /*
        @brief Actualiza las físicas de Box2d
     */
    void step();

    /*
        @param bodyDef: Cuerpo a ser destruido

        @brief Destruye el cuerpo del mundo de Box2d
     */
    void destroy_body(b2Body* bodyDef);

    /*
        @param no_wind_cheat_activated: verdadero en caso de estar activado el cheat de 0 viento, falso en caso contrario

        @brief Genera una nueva fuerza de viento de forma aleatoria, en el caso de no_wind_cheat_activated ser falso
     */
    void newWindForce(const bool& no_wind_cheat_activated);

    friend class InfoParser;

    ~Engine();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    Engine(Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;
};

#endif  // WORMS2D_BATTLEFIELD_H
