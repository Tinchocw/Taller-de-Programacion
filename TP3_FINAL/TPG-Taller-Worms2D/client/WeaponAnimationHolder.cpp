#include "WeaponAnimationHolder.h"

void WeaponAnimationHolder::load_all_draw_animations(TexturesPool& pool) {
    weapon_draw_animations.insert(
            {WeaponsAndTools::BAZOOKA,
             std::make_unique<Animation>(pool.get_draw_texture(WeaponsDraw::WORM_DRAW_BAZOOKA), 7,
                                         2, false)});

    weapon_draw_animations.insert(
            {WeaponsAndTools::MORTAR,
             std::make_unique<Animation>(pool.get_draw_texture(WeaponsDraw::WORM_DRAW_MORTAR), 7, 2,
                                         false)});

    weapon_draw_animations.insert(
            {WeaponsAndTools::DYNAMITE,
             std::make_unique<Animation>(pool.get_draw_texture(WeaponsDraw::WORM_DRAW_DYNAMITE), 10,
                                         2, false)});

    weapon_draw_animations.insert(
            {WeaponsAndTools::GREEN_GRENADE,
             std::make_unique<Animation>(
                     pool.get_draw_texture(WeaponsDraw::WORM_DRAW_GREEN_GRENADE), 10, 2, false)});

    weapon_draw_animations.insert(
            {WeaponsAndTools::RED_GRENADE,
             std::make_unique<Animation>(pool.get_draw_texture(WeaponsDraw::WORM_DRAW_RED_GRENADE),
                                         10, 2, false)});


    weapon_draw_animations.insert(
            {WeaponsAndTools::BANANA,
             std::make_unique<Animation>(pool.get_draw_texture(WeaponsDraw::WORM_DRAW_BANANA), 10,
                                         2, false)});

    weapon_draw_animations.insert(
            {WeaponsAndTools::HOLY_GRENADE,
             std::make_unique<Animation>(pool.get_draw_texture(WeaponsDraw::WORM_DRAW_HOLY_GRENADE),
                                         10, 2, false)});

    weapon_draw_animations.insert(
            {WeaponsAndTools::BASEBALL_BAT,
             std::make_unique<Animation>(pool.get_draw_texture(WeaponsDraw::WORM_DRAW_BASEBALL_BAT),
                                         10, 2, false)});

    weapon_draw_animations.insert(
            {WeaponsAndTools::AIR_STRIKE,
             std::make_unique<Animation>(pool.get_draw_texture(WeaponsDraw::WORM_DRAW_AIR_STRIKE),
                                         10, 2, false)});

    weapon_draw_animations.insert(
            {WeaponsAndTools::TELEPORT,
             std::make_unique<Animation>(pool.get_draw_texture(WeaponsDraw::WORM_DRAW_TELEPORT), 10,
                                         2, false)});
}

WeaponAnimationHolder::WeaponAnimationHolder(TexturesPool& pool):
        current_weapon(WeaponsAndTools::BAZOOKA),
        aim_animations(pool),
        weapon_drawn_frames_counter(0) {
    this->load_all_draw_animations(pool);
}

void WeaponAnimationHolder::update(const float& new_inclination_degrees, const bool& charging_power,
                                   const WeaponsAndTools& equipped_weapon,
                                   const bool& weapon_currently_stored) {
    if (weapon_currently_stored) {
        weapon_drawn_frames_counter = 0;
    }
    current_weapon = equipped_weapon;
    weapon_draw_animations[current_weapon]->update(weapon_currently_stored);
    aim_animations.update(new_inclination_degrees, charging_power);
}

void WeaponAnimationHolder::render(SDL2pp::Renderer& renderer, SDL2pp::Rect& dest,
                                   const SDL_RendererFlip& flipType, const double& angle) {

    if (current_weapon == WeaponsAndTools::BASEBALL_BAT) {
        dest.SetW(48);
    }

    if (weapon_drawn_frames_counter < 14 /* 7*2 */) {
        weapon_draw_animations[current_weapon]->render(renderer, dest, 0, 0, flipType);
        weapon_drawn_frames_counter++;
    } else {
        aim_animations.render(this->current_weapon, renderer, dest, flipType, angle);
    }
}
