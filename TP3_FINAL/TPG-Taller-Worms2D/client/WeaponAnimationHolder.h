#ifndef WEAPONHOLDANIMATION_H
#define WEAPONHOLDANIMATION_H

#include <memory>
#include <unordered_map>

#include <SDL2pp/SDL2pp.hh>

#include "AimAnimation.h"
#include "Animation.h"
#include "TexturesPool.h"

class WeaponAnimationHolder {
private:
    /** Current weapon. */
    WeaponsAndTools current_weapon;
    /** Map of SDL actors_textures of the weapon draw animations. */
    std::unordered_map<WeaponsAndTools, std::shared_ptr<Animation>> weapon_draw_animations;
    /** Weapons aim animations*/
    AimAnimation aim_animations;
    /** Indicates if the weapon was already animated thw draw. */
    unsigned int weapon_drawn_frames_counter;

    void load_all_draw_animations(TexturesPool& pool);

public:
    explicit WeaponAnimationHolder(TexturesPool& pool);

    /**
     * @brief Updates the current weapon.
     *
     * @param new_inclination_degrees New inclination of the weapon.
     * @param charging_power Indicates if the weapon is charging power.
     * @param equipped_weapon Weapon that is equipped.
     * @param weapon_currently_stored Indicates if the weapon is currently stored.
     */
    void update(const float& new_inclination_degrees, const bool& charging_power,
                const WeaponsAndTools& equipped_weapon, const bool& weapon_currently_stored);

    /**
     *  @brief Renders the current weapon.
     * @param renderer Renderer to render the texture.
     * @param dest Destination of the texture
     * @param flipType Flip type to apply to the texture
     * @param angle Angle to rotate the texture.
     */
    void render(SDL2pp::Renderer& renderer, SDL2pp::Rect& dest,
                const SDL_RendererFlip& flipType = SDL_FLIP_HORIZONTAL, const double& angle = 0.0);
};


#endif  // WEAPONHOLDANIMATION_H
