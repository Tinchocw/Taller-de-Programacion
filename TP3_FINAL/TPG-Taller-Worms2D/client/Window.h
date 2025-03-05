#ifndef WINDOW_H
#define WINDOW_H

#include <memory>
#include <unordered_map>

#include <SDL2pp/SDL2pp.hh>

#include "../common/const.h"

#include "Animation.h"
#include "text_printer.h"

class TexturesPool;
class Camera;
class TextPrinter;

class Window {
private:
    std::shared_ptr<SDL2pp::Window> game_window;
    std::shared_ptr<SDL2pp::Renderer> game_renderer;

    std::unordered_map<TerrainActors, std::shared_ptr<SDL2pp::Texture>&> background_textures;

    std::unordered_map<bool, Animation> end_of_game_animations;

    void render_stage_texture(const std::shared_ptr<SDL2pp::Texture>& texture,
                              const SDL2pp::Rect& destination);

    unsigned int ticks_since_end_of_game;

public:
    Window(const int& width, const int& height);

    std::shared_ptr<SDL2pp::Renderer>& get_renderer();

    /// Loads background and general textures the window uses
    void load_base_textures(TexturesPool& pool);

    /// Clears the actors_textures in screen calling the SDL2pp::Renderer::Clear method
    void clear_textures();

    /// Presents the actors_textures in screen calling the SDL2pp::Renderer::Present method
    void present_textures();

    void render_background(TexturesPool& pool, Camera& camera);

    bool render_end_of_game_texture(const bool& won_game, TextPrinter& text_printer);

    ~Window() = default;

    friend class TexturesPool;
};


#endif  // WINDOW_H
