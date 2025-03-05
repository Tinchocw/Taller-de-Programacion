#ifndef TEXT_PRINTER_H
#define TEXT_PRINTER_H

#include <memory>
#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "TexturesPool.h"

class TextPrinter {
private:
    SDL2pp::Font font;
    std::shared_ptr<SDL2pp::Texture>& background;
    unsigned int font_size;

    static SDL_Color interpret_team_colour(const TeamColours& team);

public:
    explicit TextPrinter(const unsigned int& font_size,
                         std::shared_ptr<SDL2pp::Texture>& font_background);

    void print_text(SDL2pp::Renderer& renderer, const std::string& text, const int& x, const int& y,
                    const int& in_line_adjustment = 0, const int& x_offset = 0,
                    const int& y_offset = 0, const bool& with_background = false,
                    const float& background_size_factor_x = 1,
                    const float& background_size_factor_y = 1);

    void print_team_text(SDL2pp::Renderer& renderer, const std::string& text, TeamColours team,
                         const int& x, const int& y, const int& in_line_adjustment = 0,
                         const int& x_offset = 0, const int& y_offset = 0,
                         const bool& with_background = false,
                         const float& background_reduce_factor_x = 1,
                         const float& background_reduce_factor_y = 1);

    ~TextPrinter() = default;
};


#endif  // TEXT_PRINTER_H
