#include "TeamResourcesHolder.h"

TeamResourcesHolder::TeamResourcesHolder(TexturesPool& pool) {
    weapons_miniatures.insert({WeaponsAndTools::TELEPORT,
                               pool.get_weapon_miniature(WeaponsAndTools::TELEPORT)});
    weapons_miniatures.insert({WeaponsAndTools::AIR_STRIKE,
                               pool.get_weapon_miniature(WeaponsAndTools::AIR_STRIKE)});
    weapons_miniatures.insert({WeaponsAndTools::HOLY_GRENADE,
                               pool.get_weapon_miniature(WeaponsAndTools::HOLY_GRENADE)});
    weapons_miniatures.insert({WeaponsAndTools::DYNAMITE,
                               pool.get_weapon_miniature(WeaponsAndTools::DYNAMITE)});
    weapons_miniatures.insert({WeaponsAndTools::BASEBALL_BAT,
                               pool.get_weapon_miniature(WeaponsAndTools::BASEBALL_BAT)});
    weapons_miniatures.insert({WeaponsAndTools::RED_GRENADE,
                               pool.get_weapon_miniature(WeaponsAndTools::RED_GRENADE)});
    weapons_miniatures.insert({WeaponsAndTools::BANANA,
                               pool.get_weapon_miniature(WeaponsAndTools::BANANA)});
    weapons_miniatures.insert({WeaponsAndTools::GREEN_GRENADE,
                               pool.get_weapon_miniature(WeaponsAndTools::GREEN_GRENADE)});
    weapons_miniatures.insert({WeaponsAndTools::MORTAR,
                               pool.get_weapon_miniature(WeaponsAndTools::MORTAR)});
    weapons_miniatures.insert({WeaponsAndTools::BAZOOKA,
                               pool.get_weapon_miniature(WeaponsAndTools::BAZOOKA)});
}

void TeamResourcesHolder::update_team(std::shared_ptr<PlayerStateG>& state) {
    if (avg_team_life.count(state->id) == 0) {
        avg_team_life.insert({state->id, state->avg_life});
        team_gadgets.insert({state->id, state->gadgets});
    } else {
        avg_team_life[state->id] = state->avg_life;
        team_gadgets[state->id] = state->gadgets;
    }

    if (state->currently_on_turn){
        current_player_in_turn = state->id;
    }
}
void TeamResourcesHolder::render_team_resources(
        const std::shared_ptr<SDL2pp::Renderer>& game_renderer, TextPrinter& text_printer) {
    //Render all weapon miniatures next to the ammo left
    //First do a ranged loop with the miniatures
    int counter_of_iterations = 0;
    for (auto& weapon_miniature : weapons_miniatures) {
        game_renderer->Copy(*weapon_miniature.second, SDL2pp::NullOpt,
                            SDL2pp::Rect(10, 10 + 25*counter_of_iterations, 25, 25));
        //Then render the ammo left

        std::string ammo_left = std::to_string(team_gadgets[current_player_in_turn]->weapon_ammo[
                weapon_miniature.first]);

        if (ammo_left == "255"){
            ammo_left = "Unlimited";
        }

        text_printer.print_text((*game_renderer), ammo_left,
                                    35, 10 + 25*counter_of_iterations, 0, 5, -5, false, 0.5, 0.5);
        counter_of_iterations++;
    }
}
