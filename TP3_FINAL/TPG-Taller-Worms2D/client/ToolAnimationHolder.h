#ifndef TOOLANIMATIONHOLDER_H
#define TOOLANIMATIONHOLDER_H

#include <memory>
#include <unordered_map>

#include <SDL2pp/SDL2pp.hh>

#include "SpecialToolAnimation.h"
#include "TexturesPool.h"
#include "audio_player.h"

class ToolAnimationHolder {
private:
    /** Current weapon/tool. */
    WeaponsAndTools currently_equipped_item;
    /** Map of SDL actors_textures of the tool animations. */
    std::unordered_map<WeaponsAndTools, std::shared_ptr<SpecialToolAnimation>> tool_animations;

    void load_all_use_animations(TexturesPool& pool);

public:
    explicit ToolAnimationHolder(TexturesPool& pool);

    void update(std::shared_ptr<WormStateG>& worm_state);

    void play_actors_animation(SDL2pp::Renderer& renderer, Camera& camera,
                               const int& non_squared_width, const int& non_squared_height,
                               const SDL_RendererFlip& flipType = SDL_FLIP_HORIZONTAL,
                               const double& angle = 0.0);

    void play_actors_sound_effects(AudioPlayer& effects_player);

    bool currently_animating_tool();
};


#endif  // TOOLANIMATIONHOLDER_H
