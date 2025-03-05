#include "TexturesPool.h"

#include <string>

template <typename EnumType>
void TexturesPool::load_texture_into_map(
        const std::string& filePath,
        std::unordered_map<EnumType, std::shared_ptr<SDL2pp::Texture>>& textureMap,
        const EnumType& texture_enum, const bool& with_alpha_blending) {

    if (with_alpha_blending) {
        textureMap.insert({texture_enum, std::make_shared<SDL2pp::Texture>(
                                                 (*renderer), SDL2pp::Surface(filePath).SetColorKey(
                                                                      true, 0x000000))});
        textureMap[texture_enum]->SetBlendMode(SDL_BLENDMODE_BLEND);
    } else {
        textureMap.insert({texture_enum, std::make_shared<SDL2pp::Texture>(
                                                 (*renderer), SDL2pp::Surface(filePath))});
    }
}

void TexturesPool::load_general_worm_textures() {
    /// Loads all the body actors_textures and enables alpha blending
    load_texture_into_map(DATA_PATH "/worms/worm-left.png", actors_textures, Actors::WORM);
    load_texture_into_map(DATA_PATH "/worms/worm-jump-up.png", actors_textures,
                          Actors::JUMPING_WORM);
    load_texture_into_map(DATA_PATH "/worms/worm-backflip.png", actors_textures,
                          Actors::BACKFLIP_WORM);
    load_texture_into_map(DATA_PATH "/worms/worm-death.png", actors_textures, Actors::DYING_WORM);
}

void TexturesPool::load_draw_textures() {
    /// Loads all the weapon draw actors_textures and enables alpha blending (If neccesary)
    load_texture_into_map(DATA_PATH "/worms/worm-bazooka-draw.png", draw_textures,
                          WeaponsDraw::WORM_DRAW_BAZOOKA, false);
    load_texture_into_map(DATA_PATH "/worms/worm-dynamite-draw.png", draw_textures,
                          WeaponsDraw::WORM_DRAW_DYNAMITE);
    load_texture_into_map(DATA_PATH "/worms/worm-ggrenade-draw.png", draw_textures,
                          WeaponsDraw::WORM_DRAW_GREEN_GRENADE);
    load_texture_into_map(DATA_PATH "/worms/worm-rgrenade-draw.png", draw_textures,
                          WeaponsDraw::WORM_DRAW_RED_GRENADE);
    load_texture_into_map(DATA_PATH "/worms/worm-banana-draw.png", draw_textures,
                          WeaponsDraw::WORM_DRAW_BANANA);
    load_texture_into_map(DATA_PATH "/worms/worm-holyg-draw.png", draw_textures,
                          WeaponsDraw::WORM_DRAW_HOLY_GRENADE);
    load_texture_into_map(DATA_PATH "/worms/worm-mortar-draw.png", draw_textures,
                          WeaponsDraw::WORM_DRAW_MORTAR);
    load_texture_into_map(DATA_PATH "/worms/worm-bat-draw.png", draw_textures,
                          WeaponsDraw::WORM_DRAW_BASEBALL_BAT);
    load_texture_into_map(DATA_PATH "/worms/worm-airstrike-draw.png", draw_textures,
                          WeaponsDraw::WORM_DRAW_AIR_STRIKE);
    load_texture_into_map(DATA_PATH "/worms/worm-teleport-draw.png", draw_textures,
                          WeaponsDraw::WORM_DRAW_TELEPORT);
}

void TexturesPool::load_aiming_textures() {
    /// Loads all the worm weapon holding actors_textures and enables alpha blending (If neccesary)
    load_texture_into_map(DATA_PATH "/worms/worm-bazooka-aim.png", aim_textures,
                          WeaponAiming::WORM_AIM_BAZOOKA);
    load_texture_into_map(DATA_PATH "/worms/worm-dynamite-aim.png", aim_textures,
                          WeaponAiming::WORM_AIM_DYNAMITE);
    load_texture_into_map(DATA_PATH "/worms/worm-ggrenade-aim.png", aim_textures,
                          WeaponAiming::WORM_AIM_GREEN_GRENADE);
    load_texture_into_map(DATA_PATH "/worms/worm-rgrenade-aim.png", aim_textures,
                          WeaponAiming::WORM_AIM_RED_GRENADE);
    load_texture_into_map(DATA_PATH "/worms/worm-banana-aim.png", aim_textures,
                          WeaponAiming::WORM_AIM_BANANA);
    load_texture_into_map(DATA_PATH "/worms/worm-holyg-aim.png", aim_textures,
                          WeaponAiming::WORM_AIM_HOLY_GRENADE);
    load_texture_into_map(DATA_PATH "/worms/worm-mortar-aim.png", aim_textures,
                          WeaponAiming::WORM_AIM_MORTAR);
    load_texture_into_map(DATA_PATH "/worms/worm-bat-aim.png", aim_textures,
                          WeaponAiming::WORM_AIM_BASEBALL_BAT);
    load_texture_into_map(DATA_PATH "/worms/worm-airstrike-aim.png", aim_textures,
                          WeaponAiming::WORM_AIM_AIR_STRIKE);
    load_texture_into_map(DATA_PATH "/worms/worm-teleport-aim.png", aim_textures,
                          WeaponAiming::WORM_AIM_TELEPORT);
}

void TexturesPool::load_projectile_textures() {
    /// Loads all the projectile textures and enables alpha blending (If neccesary)
    load_texture_into_map(DATA_PATH "/weapons/bazooka-missile.png", projectile_textures,
                          Projectiles::BAZOOKA_PROYECTILE, false);
    load_texture_into_map(DATA_PATH "/weapons/mortar-missile.png", projectile_textures,
                          Projectiles::MORTAR_PROYECTILE, false);
    load_texture_into_map(DATA_PATH "/weapons/mortar-fragment.png", projectile_textures,
                          Projectiles::MORTAR_FRAGMENT);
    load_texture_into_map(DATA_PATH "/weapons/green-grenade-projectile.png", projectile_textures,
                          Projectiles::GREEN_GRENADE_PROYECTILE);
    load_texture_into_map(DATA_PATH "/weapons/red-grenade-projectile.png", projectile_textures,
                          Projectiles::RED_GRENADE_PROYECTILE);
    load_texture_into_map(DATA_PATH "/weapons/spinning-banana.png", projectile_textures,
                          Projectiles::BANANA_PROYECTILE);
    load_texture_into_map(DATA_PATH "/weapons/holy-grenade-projectile.png", projectile_textures,
                          Projectiles::HOLY_GRENADE_PROYECTILE);
    load_texture_into_map(DATA_PATH "/weapons/dynamite-projectile.png", projectile_textures,
                          Projectiles::DYNAMITE_PROYECTILE);
    load_texture_into_map(DATA_PATH "/weapons/airstrike-projectile.png", projectile_textures,
                          Projectiles::AIR_STRIKE_PROYECTILE);
}

void TexturesPool::load_effect_textures() {
    load_texture_into_map(DATA_PATH "/weapons/bazooka-explosion.png", effect_textures,
                          VisualEffects::NORMAL_EXPLOSION);
    load_texture_into_map(DATA_PATH "/weapons/mortar-fragment-explosion.png", effect_textures,
                          VisualEffects::FRAGMENT_EXPLOSION);
    load_texture_into_map(DATA_PATH "/stage/heal-crate-effect.png", effect_textures,
                          VisualEffects::CRATE_HEAL);
    load_texture_into_map(DATA_PATH "/stage/ammo-crate-effect.png", effect_textures,
                          VisualEffects::CRATE_AMMO);
}

void TexturesPool::load_tool_use_textures() {
    load_texture_into_map(DATA_PATH "/weapons/worm-leaving-teleport.png", tool_usage_textures,
                          ToolUsage::TELEPORT_USE);
    load_texture_into_map(DATA_PATH "/weapons/worm-airstrike-call.png", tool_usage_textures,
                          ToolUsage::AIR_STRIKE_CALL);
    load_texture_into_map(DATA_PATH "/weapons/worm-bat-swing.png", tool_usage_textures,
                          ToolUsage::BASEBALL_SWING);
}

void TexturesPool::load_combat_textures() {
    load_texture_into_map(DATA_PATH "/weapons/crosshair.png", actors_textures, Actors::CROSSHAIR);
    load_texture_into_map(DATA_PATH "/miscellaneous/font-name-background.png", actors_textures,
                          Actors::STATE_SIGN, false);
    load_texture_into_map(DATA_PATH "/miscellaneous/timer-font-background.png", actors_textures,
                          Actors::TIMER_SIGN, false);
    load_texture_into_map(DATA_PATH "/weapons/loading-shot.png", actors_textures,
                          Actors::POWER_CHARGE_BAR);
}

void TexturesPool::load_worm_textures() {
    this->load_general_worm_textures();
    this->load_draw_textures();
    this->load_aiming_textures();
}

void TexturesPool::load_general_textures() {
    /// Loads all the level actors_textures and enables alpha blending
    load_texture_into_map(DATA_PATH "/miscellaneous/win_sprite.png", level_actors_textures,
                          TerrainActors::WIN_SIGN);
    load_texture_into_map(DATA_PATH "/miscellaneous/lose_sprite.png", level_actors_textures,
                          TerrainActors::LOOSE_SIGN);
    load_texture_into_map(DATA_PATH "/stage/bar.png", level_actors_textures,
                          TerrainActors::BAR);
    load_texture_into_map(DATA_PATH "/stage/long-bar.png", level_actors_textures,
                          TerrainActors::LONG_BAR);
    load_texture_into_map(DATA_PATH "/stage/airjet.png", level_actors_textures,
                          TerrainActors::AIR_JET);
    load_texture_into_map(DATA_PATH "/blue-water-sprites/blue_water.png", level_actors_textures,
                          TerrainActors::WATER);
    load_texture_into_map(DATA_PATH "/blue-water-sprites/deep-water.png", level_actors_textures,
                          TerrainActors::DEEP_WATER);
    load_texture_into_map(DATA_PATH "/stage/mystery-crate-floor.png", level_actors_textures,
                          TerrainActors::CRATE);
    load_texture_into_map(DATA_PATH "/stage/mystery-crate-falling3.png", level_actors_textures,
                          TerrainActors::CRATE_FALLING);
    load_texture_into_map(DATA_PATH "/stage/windl.png", actors_textures,
                          Actors::WIND_LEFT, false);
    load_texture_into_map(DATA_PATH "/stage/windr.png", actors_textures,
                          Actors::WIND_RIGHT, false);
}

void TexturesPool::load_weapon_miniature_textures() {
    load_texture_into_map(DATA_PATH "/weapons-icons/bazooka.png", miniature_weapons,
                          WeaponsAndTools::BAZOOKA);
    load_texture_into_map(DATA_PATH "/weapons-icons/mortar.png", miniature_weapons,
                          WeaponsAndTools::MORTAR);
    load_texture_into_map(DATA_PATH "/weapons-icons/green.png", miniature_weapons,
                          WeaponsAndTools::GREEN_GRENADE);
    load_texture_into_map(DATA_PATH "/weapons-icons/red.png", miniature_weapons,
                          WeaponsAndTools::RED_GRENADE);
    load_texture_into_map(DATA_PATH "/weapons-icons/dynamite.png", miniature_weapons,
                          WeaponsAndTools::DYNAMITE);
    load_texture_into_map(DATA_PATH "/weapons-icons/holy.png", miniature_weapons,
                          WeaponsAndTools::HOLY_GRENADE);
    load_texture_into_map(DATA_PATH "/weapons-icons/baseball.png", miniature_weapons,
                          WeaponsAndTools::BASEBALL_BAT);
    load_texture_into_map(DATA_PATH "/weapons-icons/airstrike.png", miniature_weapons,
                          WeaponsAndTools::AIR_STRIKE);
    load_texture_into_map(DATA_PATH "/weapons-icons/teleport.png", miniature_weapons,
                          WeaponsAndTools::TELEPORT);
    load_texture_into_map(DATA_PATH "/weapons-icons/banana.png", miniature_weapons,
                          WeaponsAndTools::BANANA);
}

void TexturesPool::load_tombstones_textures() {
    /// Loads all the tombstones textures and enables alpha blending (If neccesary)
    load_texture_into_map(DATA_PATH "/miscellaneous/tombstone1.png", tombstones_textures,
                          Tombstones::TOMBSTONE_1);
    load_texture_into_map(DATA_PATH "/miscellaneous/tombstone2.png", tombstones_textures,
                          Tombstones::TOMBSTONE_2);
    load_texture_into_map(DATA_PATH "/miscellaneous/tombstone3.png", tombstones_textures,
                          Tombstones::TOMBSTONE_3);
    load_texture_into_map(DATA_PATH "/miscellaneous/tombstone4.png", tombstones_textures,
                          Tombstones::TOMBSTONE_4);
    load_texture_into_map(DATA_PATH "/miscellaneous/tombstone1.png", tombstones_textures,
                          Tombstones::TOMBSTONE_5);
    load_texture_into_map(DATA_PATH "/miscellaneous/tombstone6.png", tombstones_textures,
                          Tombstones::TOMBSTONE_6);
}

TexturesPool::TexturesPool(std::shared_ptr<SDL2pp::Renderer>& game_renderer):
        renderer(game_renderer) {
    load_worm_textures();
    load_general_textures();
    load_combat_textures();
    load_projectile_textures();
    load_tool_use_textures();
    load_effect_textures();
    load_weapon_miniature_textures();
    load_tombstones_textures();
}

void TexturesPool::load_level_textures(const std::string& level_name){
    /// Loads all the level actors_textures and enables alpha blending
    load_texture_into_map(DATA_PATH "/stage/" + level_name + "-gradient.png", level_actors_textures,
                          TerrainActors::GRADIENT, false);
    load_texture_into_map(DATA_PATH "/stage/" + level_name + "-background.png", level_actors_textures,
                          TerrainActors::BACKGROUND);
}

std::shared_ptr<SDL2pp::Texture>& TexturesPool::get_actor_texture(const Actors& actor_to_fetch) {
    return (actors_textures[actor_to_fetch]);
}

std::shared_ptr<SDL2pp::Texture>& TexturesPool::get_level_texture(
        const TerrainActors& level_actor_to_fetch) {
    return (level_actors_textures[level_actor_to_fetch]);
}

std::shared_ptr<SDL2pp::Texture>& TexturesPool::get_aim_texture(
        const WeaponAiming& aim_texture_to_fetch) {
    return (aim_textures[aim_texture_to_fetch]);
}

std::shared_ptr<SDL2pp::Texture>& TexturesPool::get_draw_texture(
        const WeaponsDraw& draw_texture_to_fetch) {
    return (draw_textures[draw_texture_to_fetch]);
}

std::shared_ptr<SDL2pp::Texture>& TexturesPool::get_projectile_texture(
        const Projectiles& projectile_texture_to_fetch) {
    return (projectile_textures[projectile_texture_to_fetch]);
}

std::shared_ptr<SDL2pp::Texture>& TexturesPool::get_tool_usage_texture(
        const ToolUsage& tool_use_texture_to_fetch) {
    return (tool_usage_textures.at(tool_use_texture_to_fetch));
}

std::shared_ptr<SDL2pp::Texture>& TexturesPool::get_effect_texture(
        const VisualEffects& effect_texture_to_fetch) {
    return (effect_textures[effect_texture_to_fetch]);
}

std::shared_ptr<SDL2pp::Texture>& TexturesPool::get_tombstone_texture(
        const Tombstones& tombstone_texture_to_fetch) {
    return (tombstones_textures[tombstone_texture_to_fetch]);
}
std::shared_ptr<SDL2pp::Texture>& TexturesPool::get_weapon_miniature(
        WeaponsAndTools weapon_to_fetch) {
    return (miniature_weapons[weapon_to_fetch]);
}
