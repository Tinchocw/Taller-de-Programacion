/*
 * Created by Federico Manuel Gomez Peter
 * Date: 17/05/18.
 */

#include "Animation.h"

#include <cassert>

#include <SDL2pp/SDL2pp.hh>

Animation::Animation(std::shared_ptr<SDL2pp::Texture>& texture,
                     const unsigned int& frames_in_texture, const unsigned int& delay_in_animation,
                     const bool& loop_animation):
        texture(texture),
        numFrames(frames_in_texture),
        size(this->texture->GetHeight() / numFrames),
        currentFrame(0),
        delay(delay_in_animation),
        loop_animation(loop_animation),
        counter(0) {
    assert(this->numFrames > 0);
    assert(this->size > 0);
}

void Animation::update(const bool& iddle) {
    if (iddle) {
        this->currentFrame = 0;
    } else {
        counter++;
        if (counter < delay) {
            return;
        } else {
            if (currentFrame < numFrames - 1) {
                this->currentFrame++;
            } else if (loop_animation) {
                this->currentFrame = 0;
            } else {
                return;  // Stuck in the last frame
            }
            counter = 0;
        }
    }
}

/**
 * @brief Renders the animation in the given coordinates.
 *
 * @param renderer Renderer.
 * @param x X coordinate.
 * @param y Y corrdinate.
 */
void Animation::render(SDL2pp::Renderer& renderer, const SDL2pp::Rect& dest,
                       const int& non_squared_width, const int& non_squared_height,
                       const SDL_RendererFlip& flipType, const double& angle) {
    renderer.Copy((*texture),
                  SDL2pp::Rect(0, (this->size) * this->currentFrame,
                               (this->size + non_squared_width), (this->size + non_squared_height)),
                  dest,
                  angle,            // don't rotate
                  SDL2pp::NullOpt,  // rotation center - not needed
                  flipType);
}
