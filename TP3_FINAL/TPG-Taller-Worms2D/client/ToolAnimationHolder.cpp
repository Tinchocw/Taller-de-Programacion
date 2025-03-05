#include "ToolAnimationHolder.h"

ToolAnimationHolder::ToolAnimationHolder(TexturesPool& pool):
        currently_equipped_item(WeaponsAndTools::BAZOOKA) {
    load_all_use_animations(pool);
}

void ToolAnimationHolder::load_all_use_animations(TexturesPool& pool) {
    tool_animations.insert({WeaponsAndTools::TELEPORT,
                            std::make_unique<TeleportAnimation>(
                                    pool.get_tool_usage_texture(ToolUsage::TELEPORT_USE), 48, 0)});

    tool_animations.insert(
            {WeaponsAndTools::AIR_STRIKE,
             std::make_unique<AirStrikeCallAnimation>(
                     pool.get_tool_usage_texture(ToolUsage::AIR_STRIKE_CALL), 10, 2)});

    tool_animations.insert(
            {WeaponsAndTools::BASEBALL_BAT,
             std::make_unique<BaseballHitAnimation>(
                     pool.get_tool_usage_texture(ToolUsage::BASEBALL_SWING), 32, 0)});
}

void ToolAnimationHolder::update(std::shared_ptr<WormStateG>& worm_state) {
    currently_equipped_item = worm_state->weapon;

    if (tool_animations.count(currently_equipped_item) != 0) {
        tool_animations.at(currently_equipped_item)->update_tool_animation(worm_state);
    }
}

void ToolAnimationHolder::play_actors_animation(SDL2pp::Renderer& renderer, Camera& camera,
                                                const int& non_squared_width,
                                                const int& non_squared_height,
                                                const SDL_RendererFlip& flipType,
                                                const double& angle) {
    tool_animations.at(currently_equipped_item)
            ->render(renderer, camera, non_squared_width, non_squared_height, flipType, angle);
}

void ToolAnimationHolder::play_actors_sound_effects(AudioPlayer& effects_player) {
    tool_animations.at(currently_equipped_item)->play_effects(effects_player);
}

bool ToolAnimationHolder::currently_animating_tool() {
    if (tool_animations.count(currently_equipped_item) != 0) {
        return (tool_animations.at(currently_equipped_item)->animation_ongoing());
    } else {
        return false;
    }
}
