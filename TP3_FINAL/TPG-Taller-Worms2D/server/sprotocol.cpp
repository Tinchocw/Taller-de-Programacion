#include "sprotocol.h"

#include <iostream>
#include <utility>
#include <vector>

#include <arpa/inet.h>
#include <errno.h>
#include <spdlog/spdlog.h>

#include "../common/States.h"
#include "../common/liberror.h"


ServerSide::Protocol::Protocol(Socket&& peer):
        peer(std::move(peer)), send_was_closed(false), recv_was_closed(false) {}

void ServerSide::Protocol::send(const void* data, unsigned int sz) {
    unsigned int sz_sent = peer.sendall(data, sz, &send_was_closed);

    if (sz_sent == 0 || send_was_closed) {
        throw LibError(errno, "se cerro la conexion con el cliente");
    }
}

void ServerSide::Protocol::recv(void* data, unsigned int sz) {
    unsigned int sz_recv = peer.recvall(data, sz, &recv_was_closed);

    if (sz_recv == 0 || recv_was_closed) {
        throw LibError(errno, "se cerro la conexion con el cliente");
    }
}


void ServerSide::Protocol::close() {
    if (not(send_was_closed || recv_was_closed)) {
        this->peer.shutdown(SHUT_RDWR);
        this->peer.close();
    }
}

// ------------------------------ RECEIVE ----------------------------------

void ServerSide::Protocol::recvGameID(int16_t& id) { id = this->recvUint8(); }

uint8_t ServerSide::Protocol::recvUint8() {
    uint8_t n;
    this->recv(&n, sizeof(uint8_t));
    return n;
}

void ServerSide::Protocol::recvCommand(Actions& c) { c = (Actions)this->recvUint8(); }

void ServerSide::Protocol::recvDirection(Direction& d) { d = (Direction)this->recvUint8(); }

void ServerSide::Protocol::recvJumpDir(JumpDir& dir) { dir = (JumpDir)this->recvUint8(); }

void ServerSide::Protocol::recvADSAngleDir(ADSAngleDir& dir) {
    dir = (ADSAngleDir)this->recvUint8();
}

void ServerSide::Protocol::recvDelay(DelayAmount& amount) {
    amount = (DelayAmount)this->recvUint8();
}

void ServerSide::Protocol::recvGadget(WeaponsAndTools& gadget) {
    gadget = (WeaponsAndTools)this->recvUint8();
}

void ServerSide::Protocol::recvString64(std::string& desc) {
    uint8_t length;
    this->recv(&length, sizeof(uint8_t));

    std::vector<char> str_net;
    str_net.resize(length + 1);  // length + '\0'
    this->recv(str_net.data(), length);
    desc = std::string(str_net.data());
}


float ServerSide::Protocol::recvFloat() {
    uint32_t raw_bits;
    recv(&raw_bits, sizeof(uint32_t));
    raw_bits = ntohl(raw_bits);
    float float_value;
    memcpy(&float_value, &raw_bits, sizeof(uint32_t));
    return float_value;
}


void ServerSide::Protocol::recvPosition(b2Vec2& position) {
    position.x = pixel_to_meter_x(recvFloat());
    position.y = pixel_to_meter_y(recvFloat());
}

// ------------------------------ SEND -----------------------------------

void ServerSide::Protocol::sendFloat(float number) {
    uint32_t number_net;
    memcpy(&number_net, &number, sizeof(uint32_t));
    number_net = htonl(number_net);
    send(&number_net, sizeof(uint32_t));
}


void ServerSide::Protocol::sendPosition(const b2Vec2& pos) {
    sendFloat(pos.x);
    sendFloat(pos.y);
}

void ServerSide::Protocol::sendString64(const std::string& str) {
    uint8_t length = str.length();
    this->send(&length, sizeof(uint8_t));
    this->send(str.data(), length);
}

void ServerSide::Protocol::sendPlayerState(const std::shared_ptr<States>& ps) {
    std::shared_ptr<PlayerStateG> p = std::dynamic_pointer_cast<PlayerStateG>(ps);

    send(&p->tag, sizeof(uint8_t));
    send(&p->id, sizeof(uint8_t));
    send(&p->currently_on_turn, sizeof(bool));
    send(&p->is_playing, sizeof(bool));

    for (const auto& [type, ammo]: p->gadgets->weapon_ammo) {
        send(&type, sizeof(uint8_t));
        send(&ammo, sizeof(uint8_t));
    }

    send(&p->avg_life, sizeof(uint8_t));
}

void ServerSide::Protocol::sendWormState(const std::shared_ptr<States>& state) {
    std::shared_ptr<WormStateG> p = std::dynamic_pointer_cast<WormStateG>(state);

    send(&p->tag, sizeof(uint8_t));
    send(&p->id, sizeof(uint8_t));
    this->sendPosition(p->pos);
    send(&p->on_turn_time, sizeof(bool));
    send(&p->weapon, sizeof(uint8_t));
    send(&p->is_walking, sizeof(bool));
    send(&p->is_jumping, sizeof(bool));
    send(&p->is_backflipping, sizeof(bool));
    send(&p->facing_right, sizeof(bool));
    send(&p->falling, sizeof(bool));
    this->sendFloat(p->aim_inclination_degrees);
    send(&p->charging_weapon, sizeof(bool));
    this->sendFloat(p->life);
    send(&p->drown, sizeof(bool));
    send(&p->using_tool, sizeof(bool));
    send(&p->team, sizeof(uint8_t));
}

void ServerSide::Protocol::sendGameInfo(const std::shared_ptr<States>& count) {
    std::shared_ptr<GameInfoL> p = std::dynamic_pointer_cast<GameInfoL>(count);

    send(&p->tag, sizeof(uint8_t));

    uint8_t length = p->description.length();
    this->send(&length, sizeof(uint8_t));
    this->send(p->description.data(), length);

    length = p->map.length();
    this->send(&length, sizeof(uint8_t));
    this->send(p->map.data(), length);

    send(&p->player_count, sizeof(uint8_t));
    send(&p->game_id, sizeof(uint8_t));
}

void ServerSide::Protocol::sendCount(const std::shared_ptr<States>& count) {
    std::shared_ptr<CountState> p = std::dynamic_pointer_cast<CountState>(count);

    send(&p->tag, sizeof(uint8_t));
    send(&p->quantity, sizeof(uint8_t));
}

void ServerSide::Protocol::sendProjectileState(const std::shared_ptr<States>& ps) {
    std::shared_ptr<ProjectileStateG> p = std::dynamic_pointer_cast<ProjectileStateG>(ps);
    send(&p->tag, sizeof(uint8_t));
    send(&p->id, sizeof(uint8_t));
    this->sendPosition(p->pos);
    this->sendFloat(p->time_till_detonation);
    this->sendFloat(p->angle);
    send(&p->type, sizeof(uint8_t));
    send(&p->impacted, sizeof(bool));
}

void ServerSide::Protocol::sendTurn(const std::shared_ptr<States>& state) {
    std::shared_ptr<PlayerTurn> turn = std::dynamic_pointer_cast<PlayerTurn>(state);
    send(&turn->tag, sizeof(uint8_t));
    send(&turn->is_your_turn, sizeof(uint8_t));
}

void ServerSide::Protocol::sendPlayerLobby(const std::shared_ptr<States>& count) {
    std::shared_ptr<PlayerStateL> p = std::dynamic_pointer_cast<PlayerStateL>(count);
    send(&p->tag, sizeof(uint8_t));
    send(&p->id, sizeof(uint8_t));
    send(&p->ready, sizeof(uint8_t));
}

void ServerSide::Protocol::sendBattlefield(const std::shared_ptr<States>& state) {
    std::shared_ptr<BattlefieldState> p = std::dynamic_pointer_cast<BattlefieldState>(state);
    send(&p->tag, sizeof(uint8_t));
    sendFloat(p->wind_force);
}

void ServerSide::Protocol::sendLevelBuild(const std::shared_ptr<States>& lb) {
    std::shared_ptr<LevelStateG> p = std::dynamic_pointer_cast<LevelStateG>(lb);
    send(&p->tag, sizeof(uint8_t));
    sendString64(p->map_name);
    send(&p->amount_of_bars, sizeof(uint8_t));

    for (const auto& bar: p->bars) {
        send(&bar.type, sizeof(uint8_t));
        sendFloat(bar.x);
        sendFloat(bar.y);
        sendFloat(bar.angle);
    }
}

void ServerSide::Protocol::sendCrate(const std::shared_ptr<States>& state) {
    std::shared_ptr<CrateState> p = std::dynamic_pointer_cast<CrateState>(state);
    send(&p->tag, sizeof(uint8_t));
    sendPosition(p->pos);
    send(&p->falling, sizeof(uint8_t));
    send(&p->was_opened, sizeof(uint8_t));
    send(&p->type, sizeof(uint8_t));
    send(&p->id, sizeof(uint8_t));
}

void ServerSide::Protocol::sendYouWin(const std::shared_ptr<States>& state) {
    std::shared_ptr<YouWin> turn = std::dynamic_pointer_cast<YouWin>(state);
    send(&turn->tag, sizeof(uint8_t));
    send(&turn->you_win, sizeof(uint8_t));
}

void ServerSide::Protocol::sendStates(const std::shared_ptr<States>& state) {
    switch (state->tag) {
        case StatesTag::GAMES_COUNT_L:
        case StatesTag::PLAYER_COUNT_L:
        case StatesTag::GAME_NOT_JOINABLE:
        case StatesTag::PROJECTILE_COUNT:
        case StatesTag::CRATE_COUNT:
            sendCount(state);
            break;

        case StatesTag::PLAYER_L:
            sendPlayerLobby(state);
            break;

        case StatesTag::INFO_GAME_L:
            sendGameInfo(state);
            break;

        case StatesTag::BATTLEFIELD_G:
            sendBattlefield(state);
            break;

        case StatesTag::PLAYER_G:
            sendPlayerState(state);
            break;

        case StatesTag::WORM_G:
            sendWormState(state);
            break;

        case StatesTag::CRATE:
            sendCrate(state);
            break;

        case StatesTag::PROJECTILE_G:
            sendProjectileState(state);
            break;

        case StatesTag::PLAYER_TURN:
            sendTurn(state);
            break;

        case StatesTag::LEVEL_BUILD:
            sendLevelBuild(state);
            break;

        case StatesTag::_YOU_WIN_:
            sendYouWin(state);
            break;

        default:
            break;
    }
}


float ServerSide::Protocol::pixel_to_meter_x(const float& pixel_position) {
    return (pixel_position / PPM);
}

float ServerSide::Protocol::pixel_to_meter_y(const float& pixel_position) {
    return ((720 - pixel_position) / PPM);
}
