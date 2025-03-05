#ifndef CAMERA_H
#define CAMERA_H

#include <memory>

#include <SDL2pp/Rect.hh>

#include "../common/config.h"
#include "../common/const.h"

#define _X_ 0
#define _Y_ 1

#define _W_ 0
#define _H_ 1

#define WIDTH 1280.0f
#define HEIGHT 720.0f

#define MAP_WIDTH 1920.0f
#define MAP_HEIGHT 1080.0f

#define MOUSE_MOVEMENT_AMPLIFIER Config::yamlNode["camera_sens"].as<float>()

enum class Priority { NONE, WORM, PROJECTILE };

struct CameraPriority {
public:
    Priority priority;
    uint8_t id;

    CameraPriority(): priority(Priority::NONE), id(0) {}
};

class Camera {
private:
    float position[2];
    float fixed_actor[2];
    /*
        @brief Valida los limites de la camara al moverse el actor
    */
    void checkBounds();
    /*
        @brief valido los limites de la camara al moverla con el mouse
    */
    void checkMouseBounds();
    /*
        @param w: Ancho del actor
        @param h: Altura del actor

        @brief Valida las dimensiones del actor
    */
    void checkActorDimensions(const float& w, const float& h);

public:
    Camera(): position{0, 0}, fixed_actor{0, 0} {}
    /*
        @param x: Posicion en x del actor
        @param y: Posicion en y del actor
        @param w: Ancho del actor
        @param h: Altura del actor

        @brief Centra la camara en el actor dado
    */
    void fixActor(const float& x, const float& y, const float& w, const float& h);
    /*
        @param x: Posicion en x del mouse
        @param y: Posicion en y del mouse

        @brief Centra la camara en la direccion del mouse dado
    */
    void fixMouse(const float& x, const float& y);
    /*
        @param x: Posicion en x del actor
        @param y: Posicion en y del actor
        @param w: Ancho del actor
        @param h: Altura del actor

        @brief Calcula un rectangulo de sdl relativo a la posicion de la camara
    */
    SDL2pp::Rect calcRect(const float& x, const float& y, const float& w, const float& h);
    /*
        @param x: Posicion en x del mouse
        @param y: Posicion en y del mouse

        @brief Calcula un rectangulo de sdl con la posicion real del mouse
    */
    SDL2pp::Rect realRect(const float& x, const float& y);

    ~Camera() = default;
};


#endif
