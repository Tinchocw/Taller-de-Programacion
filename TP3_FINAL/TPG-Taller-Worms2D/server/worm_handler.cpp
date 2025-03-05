#include "worm_handler.h"

#include <algorithm>
#include <numeric>
#include <random>

#include "Player.h"
#include "gadget.h"
#include "turn_handler.h"
#include "worm.h"

WormHandler::WormHandler(std::map<uint8_t, std::unique_ptr<Player>>& players): players(players) {}

void WormHandler::updateTurnWorm(const uint8_t& id, const uint8_t& worm_id) {
    turn_worm = players.at(id)->worms.at(worm_id);
}

void WormHandler::player_start_moving(const Direction& direction, const uint8_t& id) {

    turn_worm->is_walking = true;
    turn_worm->facing_right = (bool)direction;
    turn_worm->move();
}

void WormHandler::player_stop_moving(const uint8_t& id) {

    turn_worm->is_walking = false;
    turn_worm->stop();
}

void WormHandler::player_jump(const JumpDir& direction, const uint8_t& id) {

    turn_worm->jump(direction);
}

void WormHandler::player_start_aiming(const ADSAngleDir& direction, const uint8_t& id) {

    turn_worm->aiming = true;
    turn_worm->aim_direction = direction;
}

void WormHandler::player_stop_aiming(const uint8_t& id) { turn_worm->aiming = false; }

void WormHandler::player_start_charging(const uint8_t& id) { turn_worm->charging_shoot = true; }

void WormHandler::player_shoot(const uint8_t& id, TurnHandler& turn_handler) {

    turn_worm->aiming = false;
    turn_worm->charging_shoot = false;

    turn_worm->shoot(turn_handler);

    turn_worm->weapon_power = 0;
    turn_worm->weapon_delay = DelayAmount::FIVE;
}

void WormHandler::player_use_clickable(b2Vec2 position, const uint8_t& id) {
    turn_worm->change_clicked_position(position);
}

void WormHandler::player_set_delay(DelayAmount delay, const uint8_t& id) {
    turn_worm->change_bullet_explosion_delay(delay);
}


void WormHandler::player_change_gadget(const WeaponsAndTools& gadget, const uint8_t& id) {
    players.at(id)->change_weapon(gadget);
}

void WormHandler::clearDamagedState() {
    for (const auto& [id, player]: players) {
        for (const auto& worm: player->worms) {
            worm.second->was_damaged = false;
        }
    }
}

void WormHandler::update_weapon(TurnHandler& turn_handler) {
    if (not turn_worm) {
        return;
    }

    if (turn_worm->aiming) {
        turn_worm->change_aim_direction();
    }
    if (turn_worm->charging_shoot) {
        turn_worm->change_fire_power(turn_handler);
    }
    turn_worm->using_tool = false;
}


void WormHandler::update_physics() {
    if (not turn_worm) {
        return;
    }

    if (turn_worm->is_walking) {
        turn_worm->move();
    }
}

void WormHandler::stop_turn_worm() {
    if (not turn_worm) {
        return;
    }

    turn_worm->stop_all();
}

const uint8_t WormHandler::players_alive() {
    uint8_t players_alive = std::accumulate(
            players.begin(), players.end(), 0,
            [](const int& sum, const auto& player) { return sum + player.second->is_playing; });

    return players_alive;
}

void WormHandler::checkDeadWorms() {
    for (const auto& [id, player]: players) {

        auto it = player->worms.cbegin();
        while (it != player->worms.cend()) {
            if (it->second->life == 0.0f) {

                {
                    auto worm_it_aux = player->worms.cbegin();
                    advance(worm_it_aux, player->worm_turn);

                    if (worm_it_aux->first >= it->first) {
                        if (player->worm_turn != 0) {
                            --player->worm_turn;
                        }
                    }
                }

                it = player->worms.erase(it);
                continue;
            }

            ++it;
        }

        if (player->worms.empty()) {
            player->is_playing = false;
        }
    }
}

void WormHandler::check_falling_worms() {
    for (const auto& [id, player]: players) {
        for (const auto& [worm_id, worm]: player->worms) {
            if ((worm->body->GetLinearVelocity().y <= 1.0f) &&
                (worm->body->GetLinearVelocity().y >= 0.0f) &&
                (worm->pos_y_before_falling < worm->body->GetPosition().y) && worm->falling) {

                worm->pos_y_before_falling = b2Max(b2Vec2(0, worm->pos_y_before_falling),
                                                   b2Vec2(0, worm->body->GetPosition().y))
                                                     .y;
            }
        }
    }
}


const bool WormHandler::allWormsStayStill() {
    for (const auto& [id, player]: players) {
        for (const auto& worm: player->worms) {
            // cppcheck-suppress useStlAlgorithm
            if (worm.second->falling) {
                return false;
            }
        }
    }

    return true;
}


void WormHandler::allWorms1HP() {
    for (const auto& [id, player]: players) {
        for (const auto& worm: player->worms) {
            worm.second->life = 1;
        }
    }
}

void WormHandler::playerAllowMultipleJump(const uint8_t& id) {
    players.at(id)->allow_multiple_jump = !players.at(id)->allow_multiple_jump;
}

void WormHandler::makePlayerWormsImmortal(const uint8_t& id) {
    players.at(id)->immortal_worms = !players.at(id)->immortal_worms;
}

void WormHandler::playerInfiniteAmmo(const uint8_t& id) { players.at(id)->infiniteAmmo(); }

void WormHandler::check_drown_worms() {
    for (const auto& [id, player]: players) {
        for (const auto& [worm_id, worm]: player->worms) {
            if (worm->getPosition().y <= 4) {
                worm->drown = true;
                worm->life = 0.0f;
                worm->was_damaged = true;
            }
        }
    }
}

void WormHandler::WW3Cheat() {
    AirStrike fake_airstrike = AirStrike();

    auto rng = std::mt19937(std::random_device{}());
    auto random = std::uniform_real_distribution<float>(10.0f, 50.0f);

    for (size_t i = 0; i < 5; i++) {
        auto x = random(rng);
        fake_airstrike.shootCheat(turn_worm->battlefield, x);
    }
}
