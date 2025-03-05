#include "AimAnimation.h"

#include <algorithm>
#include <memory>

void AimAnimation::load_all_aim_textures(TexturesPool& pool) {
    // Non aimable weapons
    weapon_non_aim_textures.insert(
            {WeaponsAndTools::DYNAMITE, (pool.get_aim_texture(WeaponAiming::WORM_AIM_DYNAMITE))});

    weapon_non_aim_textures.insert({WeaponsAndTools::AIR_STRIKE,
                                (pool.get_aim_texture(WeaponAiming::WORM_AIM_AIR_STRIKE))});

    weapon_non_aim_textures.insert(
            {WeaponsAndTools::TELEPORT, (pool.get_aim_texture(WeaponAiming::WORM_AIM_TELEPORT))});

    // Aimable weapons
    weapon_aim_textures.insert(
            {WeaponsAndTools::BAZOOKA, (pool.get_aim_texture(WeaponAiming::WORM_AIM_BAZOOKA))});

    weapon_aim_textures.insert(
            {WeaponsAndTools::MORTAR, (pool.get_aim_texture(WeaponAiming::WORM_AIM_MORTAR))});

    weapon_aim_textures.insert({WeaponsAndTools::GREEN_GRENADE,
                                (pool.get_aim_texture(WeaponAiming::WORM_AIM_GREEN_GRENADE))});

    weapon_aim_textures.insert({WeaponsAndTools::RED_GRENADE,
                                (pool.get_aim_texture(WeaponAiming::WORM_AIM_RED_GRENADE))});

    weapon_aim_textures.insert(
            {WeaponsAndTools::BANANA, (pool.get_aim_texture(WeaponAiming::WORM_AIM_BANANA))});

    weapon_aim_textures.insert({WeaponsAndTools::HOLY_GRENADE,
                                (pool.get_aim_texture(WeaponAiming::WORM_AIM_HOLY_GRENADE))});

    weapon_aim_textures.insert({WeaponsAndTools::BASEBALL_BAT,
                                (pool.get_aim_texture(WeaponAiming::WORM_AIM_BASEBALL_BAT))});
}

AimAnimation::AimAnimation(TexturesPool& pool):
        crosshair_texture(pool.get_actor_texture(Actors::CROSSHAIR)),
        power_charge_animation(std::make_unique<Animation>(
                pool.get_actor_texture(Actors::POWER_CHARGE_BAR), 6, 2, false)),
        current_inclination(0) {
    this->load_all_aim_textures(pool);
}

void AimAnimation::render(const WeaponsAndTools& current_weapon, SDL2pp::Renderer& renderer,
                          SDL2pp::Rect& dest, const SDL_RendererFlip& flipType,
                          const double& angle) {

    // Las animaciones aim tienen 32 frames

    if (current_weapon == WeaponsAndTools::BASEBALL_BAT) {
        dest.SetW(50);
        renderer.Copy((*weapon_aim_textures.at(current_weapon)),
                      SDL2pp::Rect(0,
                                   std::min((16 * 60 - (60) * int(-1 * ((this->current_inclination /
                                                                         (M_PI / 32))))),
                                            1860),
                                   50, 60),
                      dest, angle, SDL2pp::NullOpt, flipType);
        return;
    }

    if (weapon_non_aim_textures.count(current_weapon) == 0) {
        renderer.Copy((*weapon_aim_textures.at(current_weapon)),
                      SDL2pp::Rect(0,
                                   std::min((16 * 60 - (60) * int(-1 * ((this->current_inclination /
                                                                         (M_PI / 32))))),
                                            1860),
                                   35, 60),
                      dest, angle, SDL2pp::NullOpt, flipType);

        if (flipType == SDL_FLIP_HORIZONTAL) {
            renderer.Copy(
                    (*crosshair_texture),
                    SDL2pp::Rect(0,
                                 std::min((16 * 60 - (60) * int(-1 * ((this->current_inclination /
                                                                       (M_PI / 32))))),
                                          1860),
                                 35, 60),
                    SDL2pp::Rect((dest.x + (40 * cos(current_inclination))),
                                 (dest.y - 40 * sin(current_inclination)), 35, 60),
                    angle, SDL2pp::NullOpt, flipType);
        } else {
            renderer.Copy(
                    (*crosshair_texture),
                    SDL2pp::Rect(0,
                                 std::min((16 * 60 - (60) * int(-1 * ((this->current_inclination /
                                                                       (M_PI / 32))))),
                                          1860),
                                 35, 60),
                    SDL2pp::Rect((dest.x - (40 * cos(current_inclination))),
                                 (dest.y - 40 * sin(current_inclination)), 35, 60),
                    angle, SDL2pp::NullOpt, flipType);
        }

        power_charge_animation->render(renderer, SDL2pp::Rect(dest.x, dest.y - 35, 35, 60), 0, 0);

    } else {
        renderer.Copy((*weapon_non_aim_textures.at(current_weapon)), SDL2pp::Rect(0, 0, 35, 60),
                      dest, angle, SDL2pp::NullOpt, flipType);
    }
}

void AimAnimation::update(const float& new_inclination_degrees, const bool& charging_power) {
    current_inclination = new_inclination_degrees;
    power_charge_animation->update(!charging_power);
}
