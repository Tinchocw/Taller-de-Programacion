#ifndef SPECIALTOOLANIMATION_H
#define SPECIALTOOLANIMATION_H

#include <memory>

#include <SDL2pp/SDL2pp.hh>

#include "common/States.h"

#include "audio_player.h"
#include "camera.h"

class SpecialToolAnimation {
protected:
    /** SDL texture of the raw image. */
    std::shared_ptr<SDL2pp::Texture>& texture;
    /** Total number of frames in the sprite. */
    unsigned int numFrames;
    /** Size of the sprite (height and width). */
    unsigned int size;
    /** Current animation frame. */
    unsigned int currentFrame;
    /** Delay between frames. */
    unsigned int delay;
    /** Counter for the animation. */
    unsigned int counter;
    /** Bool that defines if a game state currently_animating_tool the tool. */
    bool currently_animating_tool;

public:
    SpecialToolAnimation(std::shared_ptr<SDL2pp::Texture>& texture,
                         const unsigned int& frames_in_texture,
                         const unsigned int& delay_in_animation):
            texture(texture),
            numFrames(frames_in_texture),
            size(this->texture->GetHeight() / numFrames),
            currentFrame(0),
            delay(delay_in_animation),
            counter(0),
            currently_animating_tool(false) {
        assert(this->numFrames > 0);
        assert(this->size > 0);
    }

    virtual void update_tool_animation(std::shared_ptr<WormStateG>& worm_state) = 0;

    virtual void render(SDL2pp::Renderer& renderer, Camera& camera, const int& non_squared_width,
                        const int& non_squared_height, const SDL_RendererFlip& flipType,
                        const double& angle) = 0;

    [[nodiscard]] bool animation_ongoing() const { return currently_animating_tool; }

    virtual void play_effects(AudioPlayer& effects_player) = 0;
};

class TeleportAnimation: public SpecialToolAnimation {
private:
    b2Vec2 initial_position;
    b2Vec2 teleport_position;

    bool alredy_played_teleport_noise;

public:
    TeleportAnimation(std::shared_ptr<SDL2pp::Texture>& texture,
                      const unsigned int& frames_in_texture,
                      const unsigned int& delay_in_animation):
            SpecialToolAnimation(texture, frames_in_texture, delay_in_animation),
            initial_position(0, 0),
            teleport_position(0, 0),
            alredy_played_teleport_noise(false) {}

    void update_tool_animation(std::shared_ptr<WormStateG>& worm_state) override {
        if (!currently_animating_tool && worm_state->using_tool) {
            currently_animating_tool = true;
            teleport_position = worm_state->pos;
        } else if (!currently_animating_tool) {
            initial_position = worm_state->pos;
            this->currentFrame = 0;
        } else {
            counter++;
            teleport_position = worm_state->pos;
            if (counter < delay) {
                return;
            } else {
                if (currentFrame < numFrames - 1) {
                    this->currentFrame++;
                } else {
                    currently_animating_tool = false;
                    return;  // Last frame, end of animation
                }
                counter = 0;
            }
        }
    }

    void render(SDL2pp::Renderer& renderer, Camera& camera, const int& non_squared_width,
                const int& non_squared_height, const SDL_RendererFlip& flipType,
                const double& angle) override {

        // Copiamos la textura del worm saliendo
        // Copio al derecho la textura, desde que sale hasta que llega
        SDL2pp::Rect render_rect_initial =
                camera.calcRect(initial_position.x, initial_position.y, 60, 60);
        renderer.Copy(
                (*texture),
                SDL2pp::Rect(0, (this->size) * this->currentFrame, (this->size + non_squared_width),
                             (this->size + non_squared_height)),
                render_rect_initial, angle, SDL2pp::NullOpt, flipType);

        // Copiamos la textura del worm llegando
        // Copio al revés la textura, desde que llega a que sale
        SDL2pp::Rect render_rect_end =
                camera.calcRect(teleport_position.x, teleport_position.y, 60, 60);
        renderer.Copy(
                (*texture),
                SDL2pp::Rect(0, ((this->size) * numFrames) - ((this->size) * this->currentFrame),
                             (this->size + non_squared_width), (this->size + non_squared_height)),
                render_rect_end, angle, SDL2pp::NullOpt, flipType);
    }

    void play_effects(AudioPlayer& effects_player) override {
        if (!alredy_played_teleport_noise) {
            effects_player.playAudio(SoundEffects::TELEPORT);
            alredy_played_teleport_noise = true;
        }
    }

    virtual ~TeleportAnimation() = default;
};

class AirStrikeCallAnimation: public SpecialToolAnimation {
private:
    b2Vec2 call_position;
    bool on_second_animation;  // Bool to perform two iterations for longer animation

    bool alredy_played_call_noise;

public:
    AirStrikeCallAnimation(std::shared_ptr<SDL2pp::Texture>& texture,
                           const unsigned int& frames_in_texture,
                           const unsigned int& delay_in_animation):
            SpecialToolAnimation(texture, frames_in_texture, delay_in_animation),
            call_position(0, 0),
            on_second_animation(false),
            alredy_played_call_noise(false) {}

    void update_tool_animation(std::shared_ptr<WormStateG>& worm_state) override {
        if (!currently_animating_tool && worm_state->using_tool) {
            currently_animating_tool = true;
            call_position = worm_state->pos;
        } else if (!currently_animating_tool) {
            this->currentFrame = 0;
        } else {
            counter++;
            call_position = worm_state->pos;
            if (counter < delay) {
                return;
            } else {
                if (currentFrame < numFrames - 1) {
                    this->currentFrame++;
                } else {
                    if (on_second_animation) {
                        currently_animating_tool = false;
                        on_second_animation = false;
                        return;  // Last frame, end of animation
                    } else {
                        currentFrame = 0;
                        on_second_animation = true;
                    }
                }
                counter = 0;
            }
        }
    }

    void render(SDL2pp::Renderer& renderer, Camera& camera, const int& non_squared_width,
                const int& non_squared_height, const SDL_RendererFlip& flipType,
                const double& angle) override {
        SDL2pp::Rect render_rect_initial =
                camera.calcRect(call_position.x, call_position.y, 40, 60);
        renderer.Copy(
                (*texture),
                SDL2pp::Rect(0, (this->size) * this->currentFrame, (this->size + non_squared_width),
                             (this->size + non_squared_height)),
                render_rect_initial, angle, SDL2pp::NullOpt, flipType);
    }

    void play_effects(AudioPlayer& effects_player) override {
        if (!alredy_played_call_noise) {
            effects_player.playAudio(SoundEffects::AIRSTRIKE);
            alredy_played_call_noise = true;
        }
    }

    virtual ~AirStrikeCallAnimation() = default;
};

class BaseballHitAnimation: public SpecialToolAnimation {
private:
    b2Vec2 use_position;

    bool alredy_played_hit_noise;

public:
    BaseballHitAnimation(std::shared_ptr<SDL2pp::Texture>& texture,
                         const unsigned int& frames_in_texture,
                         const unsigned int& delay_in_animation):
            SpecialToolAnimation(texture, frames_in_texture, delay_in_animation),
            use_position(0, 0),
            alredy_played_hit_noise(false) {}

    void update_tool_animation(std::shared_ptr<WormStateG>& worm_state) override {
        if (!currently_animating_tool && worm_state->using_tool) {
            currently_animating_tool = true;
        } else if (!currently_animating_tool) {
            this->currentFrame = 0;
            use_position = worm_state->pos;
        } else {
            counter++;
            if (counter < delay) {
                return;
            } else {
                if (currentFrame < numFrames - 1) {
                    this->currentFrame++;
                } else {
                    currently_animating_tool = false;
                    return;  // Last frame, end of animation
                }
                counter = 0;
            }
        }
    }

    void render(SDL2pp::Renderer& renderer, Camera& camera, const int& non_squared_width,
                const int& non_squared_height, const SDL_RendererFlip& flipType,
                const double& angle) override {
        SDL2pp::Rect render_rect_initial = camera.calcRect(use_position.x, use_position.y, 50, 60);
        renderer.Copy(
                (*texture),
                SDL2pp::Rect(0, (this->size) * this->currentFrame, (this->size + non_squared_width),
                             (this->size + non_squared_height)),
                render_rect_initial, angle, SDL2pp::NullOpt, flipType);
    }

    void play_effects(AudioPlayer& effects_player) override {
        if (!alredy_played_hit_noise) {
            effects_player.playAudio(SoundEffects::BASEBALL_JINGLE);
            effects_player.playAudio(SoundEffects::BASEBALL_IMPACT);
            alredy_played_hit_noise = true;
        }
    }

    virtual ~BaseballHitAnimation() = default;
};


#endif  // SPECIALTOOLANIMATION_H
