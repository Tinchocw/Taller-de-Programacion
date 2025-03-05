#ifndef GAMEACTOR_H
#define GAMEACTOR_H

#include <memory>
#include <random>
#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "../common/States.h"
#include "../common/const.h"
#include "box2d/b2_math.h"

#include "Animation.h"
#include "TexturesPool.h"
#include "ToolAnimationHolder.h"
#include "WeaponAnimationHolder.h"
#include "audio_player.h"
#include "camera.h"
#include "death_animation.h"
#include "text_printer.h"

#define NAMES_IN_FILE Config::wormNames["amount_of_names"].as<int>()

// ----------------------- ACTOR INTERFACE ----------------------

class GameActor {
protected:
    b2Vec2 position;
    Camera& camera;

public:
    explicit GameActor(const float& x, const float& y, Camera& camera):
            position(x, y), camera(camera) {}
    virtual void render(std::shared_ptr<SDL2pp::Renderer>& game_renderer) = 0;
    virtual void update(std::shared_ptr<States>& actor_state) = 0;
    virtual void print_state(std::shared_ptr<SDL2pp::Renderer>& game_renderer,
                             TextPrinter& state_printer) = 0;
    virtual void play_state_audio(AudioPlayer& effects_player) = 0;
    virtual ~GameActor() = default;
};

// ----------------------- WORM ----------------------

class Worm: public GameActor {
private:
    WeaponsAndTools equipped_weapon;
    bool on_turn_time;
    bool is_walking;
    bool is_jumping;
    bool is_backflipping;
    bool facing_right;
    bool falling;
    bool using_tool;
    float life_points_remaining;

    float aim_inclination_degrees;

    TeamColours team_colour;
    std::string worm_name;

    Animation walking;
    Animation jumping;
    Animation backflipping;

    DeathAnimation dead;

    WeaponAnimationHolder weapon_animations;

    ToolAnimationHolder tool_usage_animations;

    uint16_t steps_counter_for_sound;
    bool alredy_played_jump_sound;
    bool alredy_played_backflip_sound;
    bool alredy_played_death_sound;

    std::string generate_random_name() {
        // Seed the random number generator
        std::random_device rd;
        std::mt19937 rng(rd());

        // Define a distribution (e.g., integers between 1 and amount_of_names)
        std::uniform_int_distribution<int> dist(0, NAMES_IN_FILE - 1);

        // Generate a random number
        int random_number = dist(rng);

        // Access the correct YAML data
        const YAML::Node& worm_names = Config::wormNames["worm-names"];

        // Return the random name
        return worm_names[random_number][0].as<std::string>();
    }

public:
    explicit Worm(std::shared_ptr<WormStateG>& initial_state, TexturesPool& pool, Camera& camera):
            GameActor(initial_state->pos.x, initial_state->pos.y, camera),
            equipped_weapon(initial_state->weapon),
            on_turn_time(initial_state->on_turn_time),
            is_walking(initial_state->is_walking),
            is_jumping(initial_state->is_jumping),
            is_backflipping(initial_state->is_backflipping),
            facing_right(initial_state->facing_right),
            falling(initial_state->falling),
            using_tool(initial_state->using_tool),
            life_points_remaining(initial_state->life),
            aim_inclination_degrees(initial_state->aim_inclination_degrees),
            team_colour(TeamColours(initial_state->team)),
            worm_name(this->generate_random_name()),

            walking(pool.get_actor_texture(Actors::WORM), 15, 1),
            jumping(pool.get_actor_texture(Actors::JUMPING_WORM), 5, 5, false),
            backflipping(pool.get_actor_texture(Actors::BACKFLIP_WORM), 22, 1, false),
            dead(pool, 0),
            weapon_animations(pool),
            tool_usage_animations(pool),
            steps_counter_for_sound(0),
            alredy_played_jump_sound(false),
            alredy_played_backflip_sound(false),
            alredy_played_death_sound(false) {}

    inline void update(std::shared_ptr<States>& actor_state) override {
        auto state = std::dynamic_pointer_cast<WormStateG>(actor_state);
        position = state->pos;
        on_turn_time = state->on_turn_time;
        equipped_weapon = state->weapon;
        is_walking = state->is_walking;
        is_jumping = state->is_jumping;
        is_backflipping = state->is_backflipping;
        facing_right = state->facing_right;
        falling = state->falling;
        using_tool = state->using_tool;
        aim_inclination_degrees = state->aim_inclination_degrees;
        life_points_remaining = state->life;

        walking.update(!is_walking);
        jumping.update(!is_jumping);
        backflipping.update(!is_backflipping);
        dead.update((life_points_remaining > 0));

        bool charging_weapon = state->charging_weapon;

        weapon_animations.update(aim_inclination_degrees, charging_weapon, equipped_weapon,
                                 (is_walking || is_jumping || is_backflipping));
        tool_usage_animations.update(state);
    }

    inline void render(std::shared_ptr<SDL2pp::Renderer>& game_renderer) override {
        SDL2pp::Rect render_rect = camera.calcRect(position.x, position.y, 32, 60);

        if (life_points_remaining == 0.0f) {
            SDL2pp::Rect death_render_rect = camera.calcRect(position.x, position.y, 60, 60);
            dead.render((*game_renderer), death_render_rect);
            return;
        }

        if (on_turn_time) {
            if (is_jumping) {
                jumping.render((*game_renderer), render_rect, 0, 0,
                               facing_right ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            } else if (is_backflipping) {
                backflipping.render((*game_renderer), render_rect, 0, 0,
                                    facing_right ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            } else if (is_walking) {
                walking.render((*game_renderer), render_rect, 0, 0,
                               facing_right ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            } else if (tool_usage_animations.currently_animating_tool()) {
                tool_usage_animations.play_actors_animation(
                        (*game_renderer), camera, 0, 0,
                        facing_right ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            } else {
                weapon_animations.render((*game_renderer), render_rect,
                                         facing_right ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            }
        } else {
            walking.render((*game_renderer), render_rect, 0, 0,
                           facing_right ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
        }
    }

    inline void print_state(std::shared_ptr<SDL2pp::Renderer>& game_renderer,
                            TextPrinter& state_printer) override {
        if (!on_turn_time) {
            SDL2pp::Rect render_rect = camera.calcRect(position.x, position.y, 32, 60);
            state_printer.print_team_text((*game_renderer),
                                          std::to_string(int(life_points_remaining)), team_colour,
                                          render_rect.x, render_rect.y, 1, 18, 30, true, 0.5, 0.5);

            state_printer.print_team_text((*game_renderer), worm_name, team_colour, render_rect.x,
                                          render_rect.y, -25, 40, 68, true,
                                          (0.75 + (worm_name.length() * 0.05)), 0.8);
        }
    }


    inline void play_state_audio(AudioPlayer& effects_player) override {

        if (tool_usage_animations.currently_animating_tool()) {
            tool_usage_animations.play_actors_sound_effects(effects_player);
            return;
        }

        if (life_points_remaining == 0.0f && !(alredy_played_death_sound)) {
            effects_player.playAudio(SoundEffects::WORM_ME_MUERO);
            alredy_played_death_sound = true;
        } else if (alredy_played_death_sound) {
            return;
        }


        if (is_jumping) {
            if (!alredy_played_jump_sound) {
                effects_player.playAudio(SoundEffects::WORM_JUMP);
                alredy_played_jump_sound = true;
            }
            return;
        } else {
            alredy_played_jump_sound = false;
        }

        if (is_backflipping) {
            if (!alredy_played_backflip_sound) {
                effects_player.playAudio(SoundEffects::WORM_BACKFLIP);
                alredy_played_backflip_sound = true;
            }
            return;
        } else {
            alredy_played_backflip_sound = false;
        }

        if (is_walking) {
            if (steps_counter_for_sound % 18 == 0) {
                effects_player.playAudio(SoundEffects::EXTEND_WALK);
            }
            steps_counter_for_sound++;
        } else {
            steps_counter_for_sound = 0;
        }
    }

    ~Worm() override = default;
};


// ----------------------- PROYECTILE INTERFACE ----------------------

class Projectile: public GameActor {
protected:
    bool impacted;
    float time_till_detonation;

public:
    explicit Projectile(std::shared_ptr<ProjectileStateG>& initial_state, TexturesPool& pool,
                        Camera& camera):
            GameActor(initial_state->pos.x, initial_state->pos.y, camera),
            impacted(false),
            time_till_detonation(10) {}
};


// ----------------------- BAZOOKA PROYECTILE ----------------------

class BazookaProjectile: public Projectile {
private:
    Animation on_air;
    Animation impact;

    float current_angle;

    bool alredy_played_impact_sound;

public:
    explicit BazookaProjectile(std::shared_ptr<ProjectileStateG>& initial_state, TexturesPool& pool,
                               Camera& camera):
            Projectile(initial_state, pool, camera),
            on_air(pool.get_projectile_texture(Projectiles::BAZOOKA_PROYECTILE), 1, 1),
            impact(pool.get_effect_texture(VisualEffects::NORMAL_EXPLOSION), 8, 3, false),
            current_angle(0),
            alredy_played_impact_sound(false) {}

    inline void update(std::shared_ptr<States>& actor_state) override {
        auto state = std::dynamic_pointer_cast<ProjectileStateG>(actor_state);
        position = state->pos;
        impacted = state->impacted;
        current_angle = state->angle;
        impact.update(!impacted);
    }

    inline void render(std::shared_ptr<SDL2pp::Renderer>& game_renderer) override {
        SDL2pp::Rect rect = camera.calcRect(position.x, position.y, 60, 60);
        if (impacted) {
            impact.render((*game_renderer), rect);
        } else {
            on_air.render((*game_renderer), rect, 0, 0, SDL_FLIP_NONE,
                          (-1 * (current_angle * 180) / M_PI));
        }
    }

    inline void print_state(std::shared_ptr<SDL2pp::Renderer>& game_renderer,
                            TextPrinter& state_printer) override {}

    inline void play_state_audio(AudioPlayer& effects_player) override {
        if (impacted && !alredy_played_impact_sound) {
            effects_player.playAudio(SoundEffects::BAZOOKA_IMPACT);
            alredy_played_impact_sound = true;
        }
    }
};

// ----------------------- MORTAR PROYECTILE ----------------------

class MortarProjectile: public Projectile {
private:
    Animation on_air;
    Animation impact;

    float current_angle;

    bool alredy_played_impact_sound;

public:
    explicit MortarProjectile(std::shared_ptr<ProjectileStateG>& initial_state, TexturesPool& pool,
                              Camera& camera):
            Projectile(initial_state, pool, camera),
            on_air(pool.get_projectile_texture(Projectiles::MORTAR_PROYECTILE), 1, 1),
            impact(pool.get_effect_texture(VisualEffects::NORMAL_EXPLOSION), 8, 3, false),
            current_angle(0),
            alredy_played_impact_sound(false) {}

    inline void update(std::shared_ptr<States>& actor_state) override {
        auto state = std::dynamic_pointer_cast<ProjectileStateG>(actor_state);
        position = state->pos;
        impacted = state->impacted;
        current_angle = state->angle;
        impact.update(!impacted);
    }

    inline void render(std::shared_ptr<SDL2pp::Renderer>& game_renderer) override {
        SDL2pp::Rect rect = camera.calcRect(position.x, position.y, 60, 60);
        if (impacted) {
            impact.render((*game_renderer), rect);
        } else {
            on_air.render((*game_renderer), rect, 0, 0, SDL_FLIP_NONE,
                          (-1 * (current_angle * 180) / M_PI));
        }
    }

    inline void print_state(std::shared_ptr<SDL2pp::Renderer>& game_renderer,
                            TextPrinter& state_printer) override {}

    inline void play_state_audio(AudioPlayer& effects_player) override {
        if (impacted && !alredy_played_impact_sound) {
            effects_player.playAudio(SoundEffects::EXPLOSION_WITH_FIRE);
            alredy_played_impact_sound = true;
        }
    }
};

// ----------------------- MORTAR FRAGMENT ----------------------

class MortarFragment: public Projectile {
private:
    Animation on_air;
    Animation impact;

    float current_angle;

    bool alredy_played_impact_sound;

public:
    explicit MortarFragment(std::shared_ptr<ProjectileStateG>& initial_state, TexturesPool& pool,
                            Camera& camera):
            Projectile(initial_state, pool, camera),
            on_air(pool.get_projectile_texture(Projectiles::MORTAR_FRAGMENT), 6, 1, true),
            impact(pool.get_effect_texture(VisualEffects::FRAGMENT_EXPLOSION), 11, 1, false),
            current_angle(0),
            alredy_played_impact_sound(false) {}

    inline void update(std::shared_ptr<States>& actor_state) override {
        auto state = std::dynamic_pointer_cast<ProjectileStateG>(actor_state);
        position = state->pos;
        impacted = state->impacted;
        current_angle = state->angle;
        impact.update(!impacted);
    }

    inline void render(std::shared_ptr<SDL2pp::Renderer>& game_renderer) override {
        SDL2pp::Rect rect = camera.calcRect(position.x, position.y, 60, 60);
        if (impacted) {
            impact.render((*game_renderer), rect);
        } else {
            on_air.render((*game_renderer), rect, 0, 0, SDL_FLIP_NONE,
                          (-1 * (current_angle * 180) / M_PI));
        }
    }

    inline void print_state(std::shared_ptr<SDL2pp::Renderer>& game_renderer,
                            TextPrinter& state_printer) override {}

    inline void play_state_audio(AudioPlayer& effects_player) override {
        if (impacted && !alredy_played_impact_sound) {
            effects_player.playAudio(SoundEffects::FRAGMENT_IMPACT);
            alredy_played_impact_sound = true;
        }
    }
};

// ----------------------- GREEN GRENADE ----------------------

class GreenGrenadeProjectile: public Projectile {
private:
    Animation on_air;
    Animation impact;

    float current_angle;

    bool alredy_played_impact_sound;

public:
    explicit GreenGrenadeProjectile(std::shared_ptr<ProjectileStateG>& initial_state,
                                    TexturesPool& pool, Camera& camera):
            Projectile(initial_state, pool, camera),
            on_air(pool.get_projectile_texture(Projectiles::GREEN_GRENADE_PROYECTILE), 1, 1),
            impact(pool.get_effect_texture(VisualEffects::NORMAL_EXPLOSION), 8, 3, false),
            current_angle(0),
            alredy_played_impact_sound(false) {}

    inline void update(std::shared_ptr<States>& actor_state) override {
        auto state = std::dynamic_pointer_cast<ProjectileStateG>(actor_state);
        position = state->pos;
        impacted = state->impacted;
        current_angle = state->angle;
        time_till_detonation = state->time_till_detonation;
        impact.update(!impacted);
    }

    inline void render(std::shared_ptr<SDL2pp::Renderer>& game_renderer) override {
        SDL2pp::Rect rect = camera.calcRect(position.x, position.y, 60, 60);
        if (impacted) {
            impact.render((*game_renderer), rect);
        } else {
            on_air.render((*game_renderer), rect, 0, 0, SDL_FLIP_NONE,
                          (-1 * (current_angle * 180) / M_PI));
        }
    }

    inline void print_state(std::shared_ptr<SDL2pp::Renderer>& game_renderer,
                            TextPrinter& state_printer) override {
        SDL2pp::Rect rect = camera.calcRect(position.x, position.y, 60, 60);
        state_printer.print_text((*game_renderer),
                                 std::to_string(static_cast<int>(time_till_detonation)), rect.x,
                                 rect.y, 0, 15, 15, true);
    }

    inline void play_state_audio(AudioPlayer& effects_player) override {
        if ((time_till_detonation <= 0.5 || impacted) && !alredy_played_impact_sound) {
            effects_player.playAudio(SoundEffects::GREEN_GRENADE_IMPACT);
            alredy_played_impact_sound = true;
        }
    }
};

// ----------------------- RED GRENADE ----------------------

class RedGrenadeProjectile: public Projectile {
private:
    Animation on_air;
    Animation impact;

    float current_angle;

    bool alredy_played_impact_sound;

public:
    explicit RedGrenadeProjectile(std::shared_ptr<ProjectileStateG>& initial_state,
                                  TexturesPool& pool, Camera& camera):
            Projectile(initial_state, pool, camera),
            on_air(pool.get_projectile_texture(Projectiles::RED_GRENADE_PROYECTILE), 1, 1),
            impact(pool.get_effect_texture(VisualEffects::NORMAL_EXPLOSION), 8, 3, false),
            current_angle(0),
            alredy_played_impact_sound(false) {}

    inline void update(std::shared_ptr<States>& actor_state) override {
        auto state = std::dynamic_pointer_cast<ProjectileStateG>(actor_state);
        position = state->pos;
        impacted = state->impacted;
        current_angle = state->angle;
        time_till_detonation = state->time_till_detonation;
        impact.update(!impacted);
    }

    inline void render(std::shared_ptr<SDL2pp::Renderer>& game_renderer) override {
        SDL2pp::Rect rect = camera.calcRect(position.x, position.y, 60, 60);
        if (impacted) {
            impact.render((*game_renderer), rect);
        } else {
            on_air.render((*game_renderer), rect, 0, 0, SDL_FLIP_NONE,
                          (-1 * (current_angle * 180) / M_PI));
        }
    }

    inline void print_state(std::shared_ptr<SDL2pp::Renderer>& game_renderer,
                            TextPrinter& state_printer) override {
        SDL2pp::Rect rect = camera.calcRect(position.x, position.y, 60, 60);
        state_printer.print_text((*game_renderer),
                                 std::to_string(static_cast<int>(time_till_detonation)), rect.x,
                                 rect.y, 0, 15, 15, true);
    }

    inline void play_state_audio(AudioPlayer& effects_player) override {
        if ((time_till_detonation <= 0.5 || impacted) && !alredy_played_impact_sound) {
            effects_player.playAudio(SoundEffects::EXPLOSION_WITH_FIRE);
            alredy_played_impact_sound = true;
        }
    }
};

// ----------------------- BANANA ----------------------

class BananaProjectile: public Projectile {
private:
    Animation on_air;
    Animation impact;

    float current_angle;

    bool alredy_played_impact_sound;

public:
    explicit BananaProjectile(std::shared_ptr<ProjectileStateG>& initial_state, TexturesPool& pool,
                              Camera& camera):
            Projectile(initial_state, pool, camera),
            on_air(pool.get_projectile_texture(Projectiles::BANANA_PROYECTILE), 1, 1),
            impact(pool.get_effect_texture(VisualEffects::NORMAL_EXPLOSION), 8, 3, false),
            current_angle(0),
            alredy_played_impact_sound(false) {}

    inline void update(std::shared_ptr<States>& actor_state) override {
        auto state = std::dynamic_pointer_cast<ProjectileStateG>(actor_state);
        position = state->pos;
        impacted = state->impacted;
        current_angle = state->angle;
        time_till_detonation = state->time_till_detonation;
        impact.update(!impacted);
    }

    inline void render(std::shared_ptr<SDL2pp::Renderer>& game_renderer) override {
        SDL2pp::Rect rect = camera.calcRect(position.x, position.y, 60, 60);
        if (impacted) {
            impact.render((*game_renderer), rect);
        } else {
            on_air.render((*game_renderer), rect, 0, 0, SDL_FLIP_NONE,
                          (-1 * (current_angle * 180) / M_PI));
        }
    }

    inline void print_state(std::shared_ptr<SDL2pp::Renderer>& game_renderer,
                            TextPrinter& state_printer) override {
        SDL2pp::Rect rect = camera.calcRect(position.x, position.y, 60, 60);
        state_printer.print_text((*game_renderer),
                                 std::to_string(static_cast<int>(time_till_detonation)), rect.x,
                                 rect.y, 0, 15, 15, true);
    }

    inline void play_state_audio(AudioPlayer& effects_player) override {
        if ((time_till_detonation <= 0.5 || impacted) && !alredy_played_impact_sound) {
            effects_player.playAudio(SoundEffects::GREEN_GRENADE_IMPACT);
            alredy_played_impact_sound = true;
        }
    }
};

// ----------------------- HOLY GRENADE ----------------------

class HolyGrenadeProjectile: public Projectile {
private:
    Animation on_air;
    Animation impact;

    float current_angle;

    bool alredy_played_choir_sound;
    bool alredy_played_impact_sound;

public:
    explicit HolyGrenadeProjectile(std::shared_ptr<ProjectileStateG>& initial_state,
                                   TexturesPool& pool, Camera& camera):
            Projectile(initial_state, pool, camera),
            on_air(pool.get_projectile_texture(Projectiles::HOLY_GRENADE_PROYECTILE), 1, 1),
            impact(pool.get_effect_texture(VisualEffects::NORMAL_EXPLOSION), 8, 3, false),
            current_angle(0),
            alredy_played_choir_sound(false),
            alredy_played_impact_sound(false) {}

    inline void update(std::shared_ptr<States>& actor_state) override {
        auto state = std::dynamic_pointer_cast<ProjectileStateG>(actor_state);
        position = state->pos;
        impacted = state->impacted;
        current_angle = state->angle;
        time_till_detonation = state->time_till_detonation;
        impact.update(!impacted);
    }

    inline void render(std::shared_ptr<SDL2pp::Renderer>& game_renderer) override {
        SDL2pp::Rect rect = camera.calcRect(position.x, position.y, 60, 60);
        if (impacted) {
            impact.render((*game_renderer), rect);
        } else {
            on_air.render((*game_renderer), rect, 0, 0, SDL_FLIP_NONE,
                          (-1 * (current_angle * 180) / M_PI));
        }
    }

    inline void print_state(std::shared_ptr<SDL2pp::Renderer>& game_renderer,
                            TextPrinter& state_printer) override {
        SDL2pp::Rect rect = camera.calcRect(position.x, position.y, 60, 60);
        state_printer.print_text((*game_renderer),
                                 std::to_string(static_cast<int>(time_till_detonation)), rect.x,
                                 rect.y, 0, 15, 15, true);
    }

    inline void play_state_audio(AudioPlayer& effects_player) override {
        if ((time_till_detonation <= 1.5) && !alredy_played_choir_sound) {
            effects_player.playAudio(SoundEffects::HOLY_GRENADE_CHOIR);
            alredy_played_choir_sound = true;
        }

        if ((time_till_detonation <= 0.5 || impacted) && !alredy_played_impact_sound) {
            effects_player.playAudio(SoundEffects::GREEN_GRENADE_IMPACT);
            alredy_played_impact_sound = true;
        }
    }
};

// ----------------------- DYNAMITE ----------------------

class DynamiteProjectile: public Projectile {
private:
    Animation countdown;
    Animation explosion;

    bool alredy_played_explosion_sound;

public:
    explicit DynamiteProjectile(std::shared_ptr<ProjectileStateG>& initial_state,
                                TexturesPool& pool, Camera& camera):
            Projectile(initial_state, pool, camera),
            countdown(pool.get_projectile_texture(Projectiles::DYNAMITE_PROYECTILE), 126, 1, false),
            explosion(pool.get_effect_texture(VisualEffects::NORMAL_EXPLOSION), 8, 3, false),
            alredy_played_explosion_sound(false) {}

    inline void update(std::shared_ptr<States>& actor_state) override {
        auto state = std::dynamic_pointer_cast<ProjectileStateG>(actor_state);
        position = state->pos;
        countdown.update();
        impacted = state->impacted;
        time_till_detonation = state->time_till_detonation;
        explosion.update(!impacted);
    }

    inline void render(std::shared_ptr<SDL2pp::Renderer>& game_renderer) override {
        SDL2pp::Rect rect = camera.calcRect(position.x, position.y, 60, 60);
        if (impacted) {
            explosion.render((*game_renderer), rect);
        } else {
            countdown.render((*game_renderer), rect);
        }
    }

    inline void print_state(std::shared_ptr<SDL2pp::Renderer>& game_renderer,
                            TextPrinter& state_printer) override {
        SDL2pp::Rect rect = camera.calcRect(position.x, position.y, 60, 60);
        state_printer.print_text((*game_renderer),
                                 std::to_string(static_cast<int>(time_till_detonation)), rect.x,
                                 rect.y, 0, 15, 15, true);
    }

    inline void play_state_audio(AudioPlayer& effects_player) override {
        if ((time_till_detonation <= 0.5) && !alredy_played_explosion_sound) {
            effects_player.playAudio(SoundEffects::GREEN_GRENADE_IMPACT);
            alredy_played_explosion_sound = true;
        }
    }
};

// ----------------------- AIR STRIKE ----------------------

class AirStrikeProjectile: public Projectile {
private:
    Animation on_air;
    Animation impact;

    float current_angle;

    bool alredy_played_impact_sound;

public:
    explicit AirStrikeProjectile(std::shared_ptr<ProjectileStateG>& initial_state,
                                 TexturesPool& pool, Camera& camera):
            Projectile(initial_state, pool, camera),
            on_air(pool.get_projectile_texture(Projectiles::AIR_STRIKE_PROYECTILE), 1, 1),
            impact(pool.get_effect_texture(VisualEffects::NORMAL_EXPLOSION), 8, 3, false),
            current_angle(0),
            alredy_played_impact_sound(false) {}

    inline void update(std::shared_ptr<States>& actor_state) override {
        auto state = std::dynamic_pointer_cast<ProjectileStateG>(actor_state);
        position = state->pos;
        impacted = state->impacted;
        current_angle = state->angle;
        impact.update(!impacted);
    }

    inline void render(std::shared_ptr<SDL2pp::Renderer>& game_renderer) override {
        SDL2pp::Rect rect = camera.calcRect(position.x, position.y, 60, 60);
        if (impacted) {
            impact.render((*game_renderer), rect);
        } else {
            on_air.render((*game_renderer), rect, 0, 0, SDL_FLIP_NONE,
                          (-1 * (current_angle * 180) / M_PI));
        }
    }

    inline void print_state(std::shared_ptr<SDL2pp::Renderer>& game_renderer,
                            TextPrinter& state_printer) override {}

    inline void play_state_audio(AudioPlayer& effects_player) override {
        if (impacted && !alredy_played_impact_sound) {
            effects_player.playAudio(SoundEffects::EXPLOSION_WITH_FIRE);
            alredy_played_impact_sound = true;
        }
    }
};

// ----------------------- CRATE INTERFACE ----------------------

class Crate: public GameActor {
protected:
    Animation falling;
    Animation on_floor;

    bool still_falling;
    bool was_opened;

    bool alredy_played_land_sound;
    bool alredy_played_open_sound;

public:
    explicit Crate(std::shared_ptr<CrateState>& initial_state, TexturesPool& pool, Camera& camera):
            GameActor(initial_state->pos.x, initial_state->pos.y, camera),
            falling(pool.get_level_texture(TerrainActors::CRATE_FALLING), 27, 2, true),
            on_floor(pool.get_level_texture(TerrainActors::CRATE), 15, 1, false),
            still_falling(true),
            was_opened(false),
            alredy_played_land_sound(false),
            alredy_played_open_sound(false) {}

    inline void print_state(std::shared_ptr<SDL2pp::Renderer>& game_renderer,
                            TextPrinter& state_printer) override {}
};

// ----------------------- TRAP CRATE ----------------------

class TrapCrate: public Crate {
private:
    Animation opening;

public:
    explicit TrapCrate(std::shared_ptr<CrateState>& initial_state, TexturesPool& pool,
                       Camera& camera):
            Crate(initial_state, pool, camera),
            opening(pool.get_effect_texture(VisualEffects::NORMAL_EXPLOSION), 8, 3, false) {}

    inline void update(std::shared_ptr<States>& actor_state) override {
        auto state = std::dynamic_pointer_cast<CrateState>(actor_state);
        position = state->pos;
        still_falling = state->falling;
        was_opened = state->was_opened;
        falling.update(!still_falling);
        on_floor.update(still_falling);
        opening.update(!was_opened);
    }

    inline void render(std::shared_ptr<SDL2pp::Renderer>& game_renderer) override {
        if (still_falling) {
            SDL2pp::Rect rect_falling = camera.calcRect(position.x, position.y, 70, 74);
            falling.render((*game_renderer), rect_falling, 0, 0, SDL_FLIP_NONE);
        } else if (was_opened) {
            SDL2pp::Rect rect_floor = camera.calcRect(position.x, position.y, 60, 60);
            opening.render((*game_renderer), rect_floor, 0, 0, SDL_FLIP_NONE);
        } else {
            SDL2pp::Rect rect_floor = camera.calcRect(position.x, position.y, 60, 60);
            on_floor.render((*game_renderer), rect_floor, 0, 0, SDL_FLIP_NONE);
        }
    }

    inline void play_state_audio(AudioPlayer& effects_player) override {
        if (was_opened && !alredy_played_land_sound) {
            effects_player.playAudio(SoundEffects::CRATE_LANDED);
            alredy_played_land_sound = true;
        }

        if (was_opened && !alredy_played_open_sound) {
            effects_player.playAudio(SoundEffects::BAZOOKA_IMPACT);
            alredy_played_open_sound = true;
        }
    }
};

class HealCrate: public Crate {
private:
    Animation opening;

public:
    explicit HealCrate(std::shared_ptr<CrateState>& initial_state, TexturesPool& pool,
                       Camera& camera):
            Crate(initial_state, pool, camera),
            opening(pool.get_effect_texture(VisualEffects::CRATE_HEAL), 13, 2, false) {}

    inline void update(std::shared_ptr<States>& actor_state) override {
        auto state = std::dynamic_pointer_cast<CrateState>(actor_state);
        position = state->pos;
        still_falling = state->falling;
        was_opened = state->was_opened;
        falling.update(!still_falling);
        on_floor.update(still_falling);
        opening.update(!was_opened);
    }

    inline void render(std::shared_ptr<SDL2pp::Renderer>& game_renderer) override {
        if (still_falling) {
            SDL2pp::Rect rect_falling = camera.calcRect(position.x, position.y, 70, 74);
            falling.render((*game_renderer), rect_falling, 0, 0, SDL_FLIP_NONE);
        } else if (was_opened) {
            SDL2pp::Rect rect_floor = camera.calcRect(position.x, (position.y - 30), 60, 60);
            opening.render((*game_renderer), rect_floor, 0, 0, SDL_FLIP_NONE);
        } else {
            SDL2pp::Rect rect_floor = camera.calcRect(position.x, position.y, 60, 60);
            on_floor.render((*game_renderer), rect_floor, 0, 0, SDL_FLIP_NONE);
        }
    }

    inline void play_state_audio(AudioPlayer& effects_player) override {
        if (was_opened && !alredy_played_land_sound) {
            effects_player.playAudio(SoundEffects::CRATE_LANDED);
            alredy_played_land_sound = true;
        }

        if (was_opened && !alredy_played_open_sound) {
            effects_player.playAudio(SoundEffects::CRATE_OPENED);
            alredy_played_open_sound = true;
        }
    }
};

class AmmoCrate: public Crate {
private:
    Animation opening;

public:
    explicit AmmoCrate(std::shared_ptr<CrateState>& initial_state, TexturesPool& pool,
                       Camera& camera):
            Crate(initial_state, pool, camera),
            opening(pool.get_effect_texture(VisualEffects::CRATE_AMMO), 21, 2, false) {}

    inline void update(std::shared_ptr<States>& actor_state) override {
        auto state = std::dynamic_pointer_cast<CrateState>(actor_state);
        position = state->pos;
        still_falling = state->falling;
        was_opened = state->was_opened;
        falling.update(!still_falling);
        on_floor.update(still_falling);
        opening.update(!was_opened);
    }

    inline void render(std::shared_ptr<SDL2pp::Renderer>& game_renderer) override {
        if (still_falling) {
            SDL2pp::Rect rect_falling = camera.calcRect(position.x, position.y, 70, 74);
            falling.render((*game_renderer), rect_falling, 0, 0, SDL_FLIP_NONE);
        } else if (was_opened) {
            SDL2pp::Rect rect_floor = camera.calcRect(position.x, (position.y - 30), 60, 60);
            opening.render((*game_renderer), rect_floor, 0, 0, SDL_FLIP_NONE);
        } else {
            SDL2pp::Rect rect_floor = camera.calcRect(position.x, position.y, 60, 60);
            on_floor.render((*game_renderer), rect_floor, 0, 0, SDL_FLIP_NONE);
        }
    }

    inline void play_state_audio(AudioPlayer& effects_player) override {
        if (was_opened && !alredy_played_land_sound) {
            effects_player.playAudio(SoundEffects::CRATE_LANDED);
            alredy_played_land_sound = true;
        }

        if (was_opened && !alredy_played_open_sound) {
            effects_player.playAudio(SoundEffects::CRATE_OPENED);
            alredy_played_open_sound = true;
        }
    }
};

#endif  // GAMEACTOR_H
