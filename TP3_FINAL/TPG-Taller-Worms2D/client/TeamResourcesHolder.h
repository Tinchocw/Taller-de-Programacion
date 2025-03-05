#ifndef TEAMRESOURCESHOLDER_H
#define TEAMRESOURCESHOLDER_H

#include <cstdint>
#include <memory>
#include <unordered_map>

#include <SDL2pp/Renderer.hh>

#include "common/States.h"

#include "text_printer.h"

class TeamResourcesHolder {
private:
    std::unordered_map<uint8_t, uint8_t> avg_team_life;
    std::unordered_map<uint8_t, std::shared_ptr<AmmoLeft>> team_gadgets;

    std::unordered_map<WeaponsAndTools, std::shared_ptr<SDL2pp::Texture>&> weapons_miniatures;

    uint8_t current_player_in_turn;

public:
    explicit TeamResourcesHolder(TexturesPool& pool);
    void update_team(std::shared_ptr<PlayerStateG>& state);
    void render_team_resources(const std::shared_ptr<SDL2pp::Renderer>& game_renderer,
                               TextPrinter& text_printer);

    ~TeamResourcesHolder() = default;
};


#endif  // TEAMRESOURCESHOLDER_H
