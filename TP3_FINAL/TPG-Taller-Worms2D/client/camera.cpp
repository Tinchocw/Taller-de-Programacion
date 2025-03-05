#include "camera.h"

#include <iostream>

#include <math.h>

void Camera::checkBounds() {
    if (position[_X_] < 0) {
        position[_X_] = 0;

    } else if (position[_X_] > MAP_WIDTH - WIDTH) {
        position[_X_] = MAP_WIDTH - WIDTH;
    }

    if (position[_Y_] > 0) {
        position[_Y_] = 0;
    }
}

void Camera::checkMouseBounds() {
    if (position[_X_] < 0) {
        position[_X_] = 0;

    } else if (position[_X_] > MAP_WIDTH - WIDTH) {
        position[_X_] = MAP_WIDTH - WIDTH;
    }

    if (position[_Y_] < -HEIGHT) {
        position[_Y_] = -HEIGHT;

    } else if (position[_Y_] > 0) {
        position[_Y_] = 0;
    }
}

void Camera::checkActorDimensions(const float& w, const float& h) {
    if (w > 0) {
        fixed_actor[_W_] = w;
    }

    if (h > 0) {
        fixed_actor[_H_] = h;
    }
}

void Camera::fixActor(const float& x, const float& y, const float& w, const float& h) {
    checkActorDimensions(w, h);

    position[_X_] = x + ((fixed_actor[_W_] - WIDTH) / 2);
    position[_Y_] = y + ((fixed_actor[_H_] - HEIGHT) / 2);

    checkBounds();
}

void Camera::fixMouse(const float& x, const float& y) {

    float _x = x - (WIDTH / 2);
    float _y = (y - (HEIGHT / 2)) * (-1);
    // -1 para adaptar la diferencia de los ejes a la camara

    if (_x == 0) {
        return;
    }

    float angle = atan2(_y, _x);

    position[_X_] += MOUSE_MOVEMENT_AMPLIFIER * (cosf(angle));
    position[_Y_] += MOUSE_MOVEMENT_AMPLIFIER * (sinf(angle) * (-1));
    // -1 para adaptar el seno a la camara que tiene los ejes invertidos

    checkMouseBounds();
}


SDL2pp::Rect Camera::calcRect(const float& x, const float& y, const float& w, const float& h) {
    return SDL2pp::Rect(x - position[_X_], y - position[_Y_], w, h);
}

SDL2pp::Rect Camera::realRect(const float& x, const float& y) {
    return SDL2pp::Rect(x + position[_X_], y + position[_Y_], 0, 0);
}
