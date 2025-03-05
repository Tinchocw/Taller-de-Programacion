#include "inputHandler.h"

#include <memory>

#include "Action.h"
#include "camera.h"

IHandler::IHandler(Queue<std::shared_ptr<Action>>& actionQ, std::atomic<bool>& quit,
                   std::atomic<bool>& my_turn, Camera& camera, std::atomic<bool>& mouse_priority,
                   std::atomic<bool>& kb_priority):
        action_queue(actionQ),
        quit(quit),
        my_turn(my_turn),
        mouse_priority(mouse_priority),
        kb_priority(kb_priority),
        camera(camera),
        clickable_gadget(false),
        grenade_selected(false) {}

void IHandler::run() {
    SDL_Event event;

    while (not quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;

            } else if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
                kb_priority = true;
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;

                    default:
                        keyDown(event.key.keysym.sym);
                        break;
                }

            } else if (event.type == SDL_KEYUP) {
                kb_priority = false;
                keyUp(event.key.keysym.sym);

            } else if (not kb_priority) {
                if (event.type == SDL_MOUSEMOTION) {
                    if (mouse_priority) {
                        camera.fixMouse(event.motion.x, event.motion.y);
                    }

                } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    if (clickable_gadget) {
                        auto rec = camera.realRect(event.motion.x, event.motion.y);
                        action_queue.push(std::make_shared<UseClickable>(rec.x, rec.y));
                    }
                }
            }
        }
    }
}


void IHandler::keyUp(const SDL_Keycode& key) {
    if (not my_turn) {
        return;
    }

    if (not clickable_gadget) {
        switch (key) {
            case SDLK_SPACE:
                this->action_queue.push(std::make_shared<Shoot>());
                return;

            case SDLK_UP:
                this->action_queue.push(std::make_shared<StopADSAngle>());
                return;

            case SDLK_DOWN:
                this->action_queue.push(std::make_shared<StopADSAngle>());
                return;

            default:
                break;
        }
    }

    switch (key) {
        case SDLK_LEFT:
            this->action_queue.push(std::make_shared<StopMoving>());
            return;

        case SDLK_RIGHT:
            this->action_queue.push(std::make_shared<StopMoving>());
            return;


        default:
            return;
    }
}

void IHandler::keyDown(const SDL_Keycode& key) {
    if (not my_turn) {
        return;
    }

    if (grenade_selected) {
        switch (key) {
            case SDLK_1:
                this->action_queue.push(std::make_shared<Delay>(DelayAmount::ONE));
                return;

            case SDLK_2:
                this->action_queue.push(std::make_shared<Delay>(DelayAmount::TWO));
                return;

            case SDLK_3:
                this->action_queue.push(std::make_shared<Delay>(DelayAmount::THREE));
                return;

            case SDLK_4:
                this->action_queue.push(std::make_shared<Delay>(DelayAmount::FOUR));
                return;

            case SDLK_5:
                this->action_queue.push(std::make_shared<Delay>(DelayAmount::FIVE));
                return;

            default:
                break;
        }
    }

    if (not clickable_gadget) {
        switch (key) {
            // Fight
            case SDLK_UP:
                this->action_queue.push(std::make_shared<ADSAngle>(ADSAngleDir::UP));
                return;

            case SDLK_DOWN:
                this->action_queue.push(std::make_shared<ADSAngle>(ADSAngleDir::DOWN));
                return;

            case SDLK_SPACE:
                this->action_queue.push(std::make_shared<FirePower>());
                return;

            default:
                break;
        }
    }

    switch (key) {
        case SDLK_LEFT:
            this->action_queue.push(std::make_shared<StartMoving>(Direction::LEFT));
            return;

        case SDLK_RIGHT:
            this->action_queue.push(std::make_shared<StartMoving>(Direction::RIGHT));
            return;

        case SDLK_RETURN:
            this->action_queue.push(std::make_shared<Jump>(JumpDir::FRONT));
            return;

        case SDLK_BACKSPACE:
            this->action_queue.push(std::make_shared<Jump>(JumpDir::BACK));
            return;

        case SDLK_F1:
            this->action_queue.push(std::make_shared<ChangeGadget>(WeaponsAndTools::BAZOOKA));
            clickable_gadget = false;
            grenade_selected = false;
            mouse_priority = true;
            return;

        case SDLK_F2:
            this->action_queue.push(std::make_shared<ChangeGadget>(WeaponsAndTools::MORTAR));
            clickable_gadget = false;
            grenade_selected = false;
            mouse_priority = true;
            return;

        case SDLK_F3:
            this->action_queue.push(std::make_shared<ChangeGadget>(WeaponsAndTools::GREEN_GRENADE));
            clickable_gadget = false;
            grenade_selected = true;
            mouse_priority = true;
            return;

        case SDLK_F4:
            this->action_queue.push(std::make_shared<ChangeGadget>(WeaponsAndTools::RED_GRENADE));
            clickable_gadget = false;
            grenade_selected = true;
            mouse_priority = true;
            return;

        case SDLK_F5:
            this->action_queue.push(std::make_shared<ChangeGadget>(WeaponsAndTools::BANANA));
            clickable_gadget = false;
            grenade_selected = true;
            mouse_priority = true;
            return;

        case SDLK_F6:
            this->action_queue.push(std::make_shared<ChangeGadget>(WeaponsAndTools::HOLY_GRENADE));
            clickable_gadget = false;
            grenade_selected = true;
            mouse_priority = true;
            return;

        case SDLK_F7:
            this->action_queue.push(std::make_shared<ChangeGadget>(WeaponsAndTools::DYNAMITE));
            clickable_gadget = false;
            grenade_selected = true;
            mouse_priority = true;
            return;

        case SDLK_F8:
            this->action_queue.push(std::make_shared<ChangeGadget>(WeaponsAndTools::BASEBALL_BAT));
            clickable_gadget = false;
            grenade_selected = false;
            mouse_priority = true;
            return;

        case SDLK_F9:
            this->action_queue.push(std::make_shared<ChangeGadget>(WeaponsAndTools::AIR_STRIKE));
            clickable_gadget = true;
            grenade_selected = false;
            mouse_priority = false;
            return;

        case SDLK_F10:
            this->action_queue.push(std::make_shared<ChangeGadget>(WeaponsAndTools::TELEPORT));
            clickable_gadget = true;
            grenade_selected = false;
            mouse_priority = false;
            return;

        default:
            return;
    }
}
