#include "text_printer.h"

TextPrinter::TextPrinter(const unsigned int& font_size,
                         std::shared_ptr<SDL2pp::Texture>& font_background):
        font(FONT_PATH "/kartooni.regular.ttf", font_size),
        background(font_background),
        font_size(font_size) {}

void TextPrinter::print_text(SDL2pp::Renderer& renderer, const std::string& text, const int& x,
                             const int& y, const int& in_line_adjustment, const int& x_offset,
                             const int& y_offset, const bool& with_background,
                             const float& background_size_factor_x,
                             const float& background_size_factor_y) {
    if (with_background) {
        renderer.Copy((*background), SDL2pp::NullOpt,
                      SDL2pp::Rect(x + in_line_adjustment, y - y_offset,
                                   background->GetWidth() * background_size_factor_x,
                                   background->GetHeight() * background_size_factor_y));
    }
    SDL2pp::Texture text_texture(renderer,
                                 font.RenderText_Solid(text, SDL_Color{221, 221, 221, 230}));

    SDL2pp::Rect text_rect(x + (x_offset / 2), y - ((y_offset * 5) / 8), text_texture.GetWidth(),
                           text_texture.GetHeight());

    renderer.Copy(text_texture, SDL2pp::NullOpt, text_rect);
}

SDL_Color TextPrinter::interpret_team_colour(const TeamColours& team) {
    switch (team) {
        case TeamColours::RED:
            return SDL_Color{221, 15, 15, 200};
        case TeamColours::BLUE:
            return SDL_Color{15, 15, 221, 200};
        case TeamColours::GREEN:
            return SDL_Color{15, 221, 15, 200};
        case TeamColours::YELLOW:
            return SDL_Color{255, 240, 10, 200};
    }
    return SDL_Color{200, 200, 200, 200};
}

void TextPrinter::print_team_text(SDL2pp::Renderer& renderer, const std::string& text,
                                  TeamColours team, const int& x, const int& y,
                                  const int& in_line_adjustment, const int& x_offset,
                                  const int& y_offset, const bool& with_background,
                                  const float& background_reduce_factor_x,
                                  const float& background_reduce_factor_y) {
    if (with_background) {
        renderer.Copy((*background), SDL2pp::NullOpt,
                      SDL2pp::Rect(x + in_line_adjustment, y - y_offset,
                                   background->GetWidth() * background_reduce_factor_x,
                                   background->GetHeight() * background_reduce_factor_y));
    }

    SDL2pp::Texture text_texture(renderer,
                                 font.RenderText_Solid(text, interpret_team_colour(team)));

    SDL2pp::Rect text_rect(x + (x_offset / 2) + in_line_adjustment, y - ((y_offset * 5) / 8),
                           text_texture.GetWidth(), text_texture.GetHeight());

    renderer.Copy(text_texture, SDL2pp::NullOpt, text_rect);
}
