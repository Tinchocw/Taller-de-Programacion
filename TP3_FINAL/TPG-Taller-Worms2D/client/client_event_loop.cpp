#include "client_event_loop.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "../common/const.h"

#include "cheatmenu.h"

#define MAX_PLAYERS Config::commonNode["max_players"].as<int>()
#define WORMS_QUANTITY Config::commonNode["worms_quantity"].as<int>()

EventLoop::EventLoop(const char* hostname, const char* servname,
                     std::unique_ptr<CheatMenu>& cheat_menu):
        quit(false),
        my_turn(false),
        runned(false),
        mouse_priority(true),
        kb_priority(false),
        loaded_base_textures(false),
        win(-1),
        protocol(hostname, servname),
        recv(this->protocol, game_state_queue, lobby_state_queue, runned),
        send(this->protocol, this->action_queue),
        input(this->action_queue, quit, my_turn, camera, mouse_priority, kb_priority),
        cheat_menu(cheat_menu) {
    spdlog::get("client")->debug("Iniciando hilo receptor en el cliente");
    recv.start();
    spdlog::get("client")->debug("Iniciando hilo sender en el cliente");
    send.start();
    cheat_menu = std::make_unique<CheatMenu>(action_queue);
    cheat_menu->hide();
}

void EventLoop::process_game_states(std::chrono::time_point<std::chrono::steady_clock>& turn_start,
                                    TexturesPool& txt_pool, TeamResourcesHolder& resources_holder) {
    std::shared_ptr<States> raw_state = nullptr;
    int expected_states =
            MAX_PLAYERS + WORMS_QUANTITY + 10;  //+ proj_count + crate_count + level + wind + turn
    for (int j = 0; j < expected_states; j++) {
        if (not game_state_queue.try_pop(raw_state)) {
            continue;
        }

        switch (raw_state->tag) {
            case StatesTag::PLAYER_G: {
                auto state = std::dynamic_pointer_cast<PlayerStateG>(raw_state);
                resources_holder.update_team(state);
                continue;
            }

            case StatesTag::BATTLEFIELD_G: {
                auto state = std::dynamic_pointer_cast<BattlefieldState>(raw_state);
                terrain_elements.update_battlefield(state);
                continue;
            }

            case StatesTag::_YOU_WIN_:
                quit = true;
                win = std::dynamic_pointer_cast<YouWin>(raw_state)->you_win;
                continue;

            case StatesTag::CRATE_COUNT:
                expected_states += std::dynamic_pointer_cast<CrateCount>(raw_state)->quantity;
                continue;

            case StatesTag::CRATE: {
                auto state = std::dynamic_pointer_cast<CrateState>(raw_state);
                if (!crates.actor_loaded(state->id)) {
                    switch (state->type) {

                        case _CrateType_::FIRST_AID:
                            crates.add_actor(state->id,
                                             std::make_shared<HealCrate>(state, txt_pool, camera));
                            continue;
                        case _CrateType_::AMMO_BOX:
                            crates.add_actor(state->id,
                                             std::make_shared<AmmoCrate>(state, txt_pool, camera));
                            continue;
                        case _CrateType_::TRAP:
                            crates.add_actor(state->id,
                                             std::make_shared<TrapCrate>(state, txt_pool, camera));
                            continue;
                    }
                    crates.delete_inactive_actors();
                } else {
                    if (state->was_opened) {
                        crates.remove_actor(state->id, raw_state);
                    } else {
                        crates.update_actor_state(state->id, raw_state);
                    }
                }
                continue;
            }

            case StatesTag::WORM_G: {
                auto state = std::dynamic_pointer_cast<WormStateG>(raw_state);
                if (!players.actor_loaded(state->id)) {
                    players.add_actor(state->id, std::make_shared<Worm>(state, txt_pool, camera));
                } else {
                    if (state->life <= 0.0f) {
                        players.remove_actor(state->id, raw_state);
                    } else {
                        players.update_actor_state(state->id, raw_state);
                    }
                }

                viewWorm(state);
                continue;
            }

            case StatesTag::PROJECTILE_COUNT:
                expected_states += std::dynamic_pointer_cast<ProjectileCount>(raw_state)->quantity;
                continue;

            case StatesTag::PROJECTILE_G: {
                auto state = std::dynamic_pointer_cast<ProjectileStateG>(raw_state);
                if (!proyectiles.actor_loaded(state->id)) {
                    switch (state->type) {
                        case (WeaponsAndTools::BAZOOKA): {
                            proyectiles.add_actor(state->id, std::make_shared<BazookaProjectile>(
                                                                     state, txt_pool, camera));
                            proyectiles.delete_inactive_actors();
                            continue;
                        }
                        case WeaponsAndTools::MORTAR: {
                            proyectiles.add_actor(state->id, std::make_shared<MortarProjectile>(
                                                                     state, txt_pool, camera));
                            proyectiles.delete_inactive_actors();
                            continue;
                        }

                        case WeaponsAndTools::GREEN_GRENADE: {
                            proyectiles.add_actor(state->id,
                                                  std::make_shared<GreenGrenadeProjectile>(
                                                          state, txt_pool, camera));
                            proyectiles.delete_inactive_actors();
                            continue;
                        }
                        case WeaponsAndTools::RED_GRENADE:
                            proyectiles.add_actor(state->id, std::make_shared<RedGrenadeProjectile>(
                                                                     state, txt_pool, camera));
                            continue;
                        case WeaponsAndTools::BANANA:
                            proyectiles.add_actor(state->id, std::make_shared<BananaProjectile>(
                                                                     state, txt_pool, camera));
                            proyectiles.delete_inactive_actors();
                            continue;
                        case WeaponsAndTools::HOLY_GRENADE:
                            proyectiles.add_actor(state->id,
                                                  std::make_shared<HolyGrenadeProjectile>(
                                                          state, txt_pool, camera));
                            proyectiles.delete_inactive_actors();
                            continue;
                        case WeaponsAndTools::DYNAMITE:
                            proyectiles.add_actor(state->id, std::make_shared<DynamiteProjectile>(
                                                                     state, txt_pool, camera));
                            proyectiles.delete_inactive_actors();
                            continue;
                        case WeaponsAndTools::MORTAR_FRAGMENT:
                            proyectiles.add_actor(state->id, std::make_shared<MortarFragment>(
                                                                     state, txt_pool, camera));
                            continue;
                        case WeaponsAndTools::BASEBALL_BAT:
                            continue;
                        case WeaponsAndTools::AIR_STRIKE:
                            proyectiles.add_actor(state->id, std::make_shared<AirStrikeProjectile>(
                                                                     state, txt_pool, camera));
                            terrain_elements.change_jet_position(state->pos.x, state->pos.y);
                            continue;
                        case WeaponsAndTools::TELEPORT:
                            continue;
                    }

                } else {
                    if (state->impacted) {
                        proyectiles.remove_actor(state->id, raw_state);
                    } else {
                        proyectiles.update_actor_state(state->id, raw_state);
                    }
                }
                viewProjectile(state);
                continue;
            }

            case StatesTag::PLAYER_TURN: {
                my_turn = std::dynamic_pointer_cast<PlayerTurn>(raw_state)->is_your_turn;
                turn_start = std::chrono::steady_clock::now();
                continue;
            }

            case StatesTag::LEVEL_BUILD: {
                auto state = std::dynamic_pointer_cast<LevelStateG>(raw_state);
                txt_pool.load_level_textures(state->map_name);
                loaded_base_textures = true;
                // Charge all the bars as terrain
                for (auto& bar: state->bars) {
                    switch (bar.type) {
                        case (TerrainActors::BAR):
                            terrain_elements.add_terrain_element(
                                    TerrainActors::BAR,
                                    std::make_unique<ShortBar>(bar.x, bar.y, bar.angle, txt_pool,
                                                               camera));
                            continue;

                        case (TerrainActors::LONG_BAR):
                            terrain_elements.add_terrain_element(
                                    TerrainActors::LONG_BAR,
                                    std::make_unique<LongBar>(bar.x, bar.y, bar.angle, txt_pool,
                                                              camera));
                            continue;

                        default:
                            continue;
                    }
                }
            }

            default:
                break;
        }
    }
}

void EventLoop::viewWorm(const std::shared_ptr<WormStateG>& worm) {
    if (camera_priority.priority == Priority::PROJECTILE) {
        return;
    }

    if ((camera_priority.id == worm->id) || (camera_priority.priority == Priority::NONE) ||
        (not players.actor_loaded(camera_priority.id))) {

        camera_priority.priority = Priority::WORM;
        camera_priority.id = worm->id;

        if ((worm->on_turn_time && not mouse_priority) || (worm->on_turn_time && kb_priority)) {
            camera.fixActor(worm->pos.x, worm->pos.y, 32, 60);
        }

        if (worm->falling || worm->is_backflipping || worm->is_jumping) {
            camera.fixActor(worm->pos.x, worm->pos.y, 32, 60);
            return;
        }

        camera_priority.priority = Priority::NONE;
    }
}

void EventLoop::viewProjectile(const std::shared_ptr<ProjectileStateG>& proj) {
    if (camera_priority.priority == Priority::WORM) {
        return;
    }

    if ((camera_priority.id == proj->id) || (camera_priority.priority == Priority::NONE)) {

        camera_priority.priority = Priority::PROJECTILE;
        camera_priority.id = proj->id;

        if (not proj->impacted) {
            camera.fixActor(proj->pos.x, proj->pos.y, 60, 60);
            return;
        }

        camera_priority.priority = Priority::NONE;
    }
}

void EventLoop::run() {
    runned = true;

    Window window(1280, 720);

    TexturesPool txt_pool(window.get_renderer());

    SDL2pp::SDL sdl(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL2pp::SDLTTF ttf;

    TextPrinter life_points_printer(15, txt_pool.get_actor_texture(Actors::STATE_SIGN));
    TextPrinter timer_printer(12, txt_pool.get_actor_texture(Actors::TIMER_SIGN));
    TextPrinter turn_time_printer(22, txt_pool.get_actor_texture(Actors::TIMER_SIGN));

    terrain_elements.load_base_terrain(txt_pool, camera);
    audio_player.play_background_music();

    TeamResourcesHolder team_resources_holder(txt_pool);

    input.start();

    int loop_start_time = SDL_GetTicks();

    std::chrono::duration<float> turn_time{};
    std::chrono::time_point<std::chrono::steady_clock> turn_start;

    while (!quit) {
        window.clear_textures();

        if (loaded_base_textures) {
            window.render_background(txt_pool, camera);
        }

        process_game_states(turn_start, txt_pool, team_resources_holder);

        turn_time = std::chrono::steady_clock::now() - turn_start;
        // tiempo restante turno = (uint8_t)(60 - turn_time)

        players.play_actors_state(window.get_renderer(), audio_player);
        players.print_actors_state(window.get_renderer(), life_points_printer);
        proyectiles.play_actors_state(window.get_renderer(), audio_player);
        proyectiles.print_actors_state(window.get_renderer(), timer_printer);
        crates.play_actors_state(window.get_renderer(), audio_player);

        terrain_elements.update_terrain();
        terrain_elements.render_terrain(window.get_renderer());

        team_resources_holder.render_team_resources(window.get_renderer(), life_points_printer);

        turn_time_printer.print_text(*(window.get_renderer()),
                                     std::to_string((int)(60 - turn_time.count())), 25, 580, -20, 0,
                                     45, true, 2.5, 2.5);

        if (my_turn) {
            turn_time_printer.print_text(*(window.get_renderer()), "Your Turn", 25, 580, 0, -30,
                                         105, false, 1, 1);
        }

        window.present_textures();

        rest(loop_start_time);
    }

    cheat_menu->close();

    // Si win == -1, suponemos que el usuario salio con el ESC asi que omitimos la animacion
    if (win < 0) {
        return;
    }

    TextPrinter end_of_game_printer(45, txt_pool.get_actor_texture(Actors::STATE_SIGN));

    while (window.render_end_of_game_texture(win, end_of_game_printer)) {
        window.present_textures();
        rest(loop_start_time);
    }
}

void EventLoop::rest(int& loop_start_time) {
    int loop_end_time = SDL_GetTicks();
    int rest_time = FRAME_DURATION - (loop_end_time - loop_start_time);

    if (rest_time < 0) {
        int time_behind = -rest_time;

        rest_time = FRAME_DURATION - time_behind % FRAME_DURATION;

        int lost = time_behind + rest_time;
        loop_start_time += lost;
    }

    SDL_Delay(rest_time);
    loop_start_time += FRAME_DURATION;
}

EventLoop::~EventLoop() {

    if (runned) {
        input.join();
    }

    recv.kill();
    send.kill();

    spdlog::get("client")->debug("Cerrando protocolo del cliente");
    protocol.close();
    spdlog::get("client")->debug("Joineando receptor en el cliente");
    recv.join();
    spdlog::get("client")->debug("Joineando sender en el cliente");
    send.join();
}
