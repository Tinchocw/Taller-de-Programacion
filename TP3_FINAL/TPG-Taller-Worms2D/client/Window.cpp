#include "Window.h"

#include "TexturesPool.h"
#include "camera.h"

Window::Window(const int& width, const int& height):
        game_window(std::make_shared<SDL2pp::Window>("Worms2D", SDL_WINDOWPOS_UNDEFINED,
                                                     SDL_WINDOWPOS_UNDEFINED, width, height,
                                                     SDL_WINDOW_RESIZABLE)),
        game_renderer(
                std::make_shared<SDL2pp::Renderer>((*game_window), -1, SDL_RENDERER_SOFTWARE)),
        ticks_since_end_of_game(0) {}

void Window::load_base_textures(TexturesPool& pool) {
    background_textures.insert(
            {TerrainActors::GRADIENT, pool.get_level_texture(TerrainActors::GRADIENT)});
    background_textures.insert(
            {TerrainActors::BACKGROUND, pool.get_level_texture(TerrainActors::BACKGROUND)});

    end_of_game_animations.insert(
            {true, Animation(pool.get_level_texture(TerrainActors::WIN_SIGN), 56, 2, true)});
    end_of_game_animations.insert(
            {false, Animation(pool.get_level_texture(TerrainActors::LOOSE_SIGN), 20, 2, true)});
}

std::shared_ptr<SDL2pp::Renderer>& Window::get_renderer() { return game_renderer; }

void Window::render_stage_texture(const std::shared_ptr<SDL2pp::Texture>& texture,
                                  const SDL2pp::Rect& destination) {

    game_renderer->Copy((*texture), SDL2pp::NullOpt, destination);
}

void Window::clear_textures() { game_renderer->Clear(); }

void Window::present_textures() { game_renderer->Present(); }

void Window::render_background(TexturesPool& pool, Camera& camera) {

    if (background_textures.empty()) {
        load_base_textures(pool);
    }

    render_stage_texture(
            background_textures.at(TerrainActors::GRADIENT),
            SDL2pp::Rect(0, 0, game_renderer->GetOutputWidth(), game_renderer->GetOutputHeight()));


    render_stage_texture(background_textures.at(TerrainActors::BACKGROUND),
                         camera.calcRect(350, 420, 1080, 200));
}

bool Window::render_end_of_game_texture(const bool& won_game, TextPrinter& text_printer) {
    if (ticks_since_end_of_game <= 200) {
        end_of_game_animations.at(won_game).update(false);
        end_of_game_animations.at(won_game).render(
                (*game_renderer), SDL2pp::Rect(0, 0, game_renderer->GetOutputWidth(),
                                               game_renderer->GetOutputHeight()));
        ticks_since_end_of_game++;

        if (won_game){
            text_printer.print_text((*game_renderer), "You WIN!", 0, 0, 0, 0, 0, false, 0.5, 0.5);
        } else{
            text_printer.print_text((*game_renderer), "You LOOSE! Keep training!",
                                    0, 0, 0, 0, 0, false, 0.5, 0.5);
        }

        return true;
    }

    return false;
}
