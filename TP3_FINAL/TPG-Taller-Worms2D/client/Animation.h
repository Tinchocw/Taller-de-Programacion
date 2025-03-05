/*
 * Created by Federico Manuel Gomez Peter
 * Date: 17/05/18.
 * Modified by Juan Manuel Pascual Osorio
 * Date: 11/23
 */
#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <memory>

#include <SDL2pp/SDL2pp.hh>
class SdlTexture;
class Area;

class Animation {
private:
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
    /** Whether the animation should loop or not. */
    bool loop_animation;
    /** Counter for the animation. */
    unsigned int counter;

public:
    /**
     * @brief Constructor of the animation abstraction.
     *
     * @param texture SDL texture of the raw image.
     * @param frames_in_texture Total number of frames in the sprite.
     * @param delay_in_animation Delay between frames.
     * @param loop_animation Whether the animation should loop or not.
     */
    explicit Animation(std::shared_ptr<SDL2pp::Texture>& texture,
                       const unsigned int& frames_in_texture,
                       const unsigned int& delay_in_animation = 0,
                       const bool& loop_animation = true);

    /**
     * @brief Updates the animation.
     *
     * @param iddle Whether the animation is iddle or not. Iddle state equals to the first frame.
     */
    void update(const bool& iddle = false);

    /**
     * @brief Renders the animation.
     *
     * @param renderer SDL renderer.
     * @param dest Destination rectangle.
     * @param non_squared_width Extra width of the sprite (For non rectangle frames).
     * @param non_squared_height Extra height of the sprite (For non rectangle frames).
     * @param flipType Flip to apply into the sprite.
     * @param angle Angle to rotate the sprite.
     */
    void render(SDL2pp::Renderer& renderer, const SDL2pp::Rect& dest,
                const int& non_squared_width = 0, const int& non_squared_height = 0,
                const SDL_RendererFlip& flipType = SDL_FLIP_HORIZONTAL, const double& angle = 0.0);

    ~Animation() = default;
};

#endif  // ANIMATION_H_
