#ifndef AIMANIMATION_H
#define AIMANIMATION_H

#include <memory>
#include <unordered_map>

#include <SDL2pp/SDL2pp.hh>

#include "common/const.h"

#include "Animation.h"
#include "TexturesPool.h"


class AimAnimation {
private:
    /** SDL texture del crosshair. */
    std::shared_ptr<SDL2pp::Texture>& crosshair_texture;
    /** Map de las texturas de apuntado de arma. */
    std::unordered_map<WeaponsAndTools, std::shared_ptr<SDL2pp::Texture>&> weapon_aim_textures;
    /** Map de las texturas de armas no apuntables. */
    std::unordered_map<WeaponsAndTools, std::shared_ptr<SDL2pp::Texture>&> weapon_non_aim_textures;
    /** Animación de cargado de poder de tiro */
    std::unique_ptr<Animation> power_charge_animation;
    /** Inclinación de aim actual. */
    float current_inclination;

    void load_all_aim_textures(TexturesPool& pool);

public:
    explicit AimAnimation(TexturesPool& pool);

    /**
     * @brief Actualiza la animación de apuntado actual.
     *
     * @param new_inclination_degrees Inclinación actual del arma.
     * @param charging_power Indica si el arma está cargando un disparo.
     */
    void update(const float& new_inclination_degrees, const bool& charging_power);

    /**
     * @brief Renderiza la animación de apuntado actual.
     *
     * @param current_weapon Arma actual.
     * @param renderer Renderer para renderizar la textura.
     * @param dest Destino de la textura.
     * @param flipType Tipo de flip a aplicar a la textura.
     * @param angle Ángulo de rotación a aplicar en la textura.
     */
    void render(const WeaponsAndTools& current_weapon, SDL2pp::Renderer& renderer,
                SDL2pp::Rect& dest, const SDL_RendererFlip& flipType = SDL_FLIP_HORIZONTAL,
                const double& angle = 0.0);
};


#endif  // AIMANIMATION_H
