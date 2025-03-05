#ifndef KEYBOARD_HANDLER_H
#define KEYBOARD_HANDLER_H

#include <atomic>
#include <memory>

#include <SDL2pp/SDL2pp.hh>

#include "../common/queue.h"
#include "../common/thread.h"

class Action;
class Camera;

class IHandler: public Thread {
private:
    Queue<std::shared_ptr<Action>>& action_queue;
    std::atomic<bool>& quit;
    std::atomic<bool>& my_turn;
    std::atomic<bool>& mouse_priority;
    std::atomic<bool>& kb_priority;
    Camera& camera;
    bool clickable_gadget;
    bool grenade_selected;
    /*
        @param key: Tecla presionada

        @brief Pushea en action_queue acciones segun la tecla presionada
    */
    void keyDown(const SDL_Keycode& key);
    /*
        @param key: Tecla soltada

        @brief Pushea en action_queue acciones segun la tecla presionada
    */
    void keyUp(const SDL_Keycode& key);

public:
    /*
        @brief Crea el IHandler con todas las referencias necesarias
    */
    explicit IHandler(Queue<std::shared_ptr<Action>>& actionQ, std::atomic<bool>& quit,
                      std::atomic<bool>& my_turn, Camera& camera, std::atomic<bool>& mouse_priority,
                      std::atomic<bool>& kb_priority);
    /*
        @brief Corre el IHandler, esperando input de teclado y de mouse a procesar
    */
    void run() override;

    ~IHandler() = default;
};


#endif
