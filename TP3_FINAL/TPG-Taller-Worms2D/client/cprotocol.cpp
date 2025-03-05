#include "cprotocol.h"

#include <cerrno>
#include <cstring>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "../common/States.h"
#include "../common/liberror.h"

void ClientSide::Protocol::send(const void* data, unsigned int sz) {
    unsigned int sz_sent = skt.sendall(data, sz, &send_was_closed);

    if (sz_sent == 0 || send_was_closed) {
        throw LibError(errno, "se cerro la conexion con el servidor");
    }
}

void ClientSide::Protocol::sendString64(const std::string& str) {
    uint8_t length = str.length();
    this->send(&length, sizeof(uint8_t));
    this->send(str.data(), length);
}

void ClientSide::Protocol::recv(void* data, unsigned int sz) {
    unsigned int sz_recv = skt.recvall(data, sz, &recv_was_closed);

    if (sz_recv == 0 || recv_was_closed) {
        throw LibError(errno, "se cerro la conexion con el servidor");
    }
}

void ClientSide::Protocol::recvCommand(Actions& c) { c = (Actions)recvUint8(); }

uint8_t ClientSide::Protocol::recvUint8() {
    uint8_t i;
    recv(&i, sizeof(uint8_t));
    return i;
}

float ClientSide::Protocol::recvFloat() {
    uint32_t raw_bits;
    recv(&raw_bits, sizeof(uint32_t));
    raw_bits = ntohl(raw_bits);
    float float_value;
    memcpy(&float_value, &raw_bits, sizeof(uint32_t));
    return float_value;
}

void ClientSide::Protocol::sendFloat(const float& number) {
    uint32_t number_net;
    memcpy(&number_net, &number, sizeof(uint32_t));
    number_net = htonl(number_net);
    send(&number_net, sizeof(uint32_t));
}

bool ClientSide::Protocol::recvBool() {
    bool bool_net;
    recv(&bool_net, sizeof(bool));
    bool bool_final = ntohl(bool_net);
    return bool_final;
}

ClientSide::Protocol::Protocol(const char* hostname, const char* servname):
        skt(hostname, servname), send_was_closed(false), recv_was_closed(false) {}

void ClientSide::Protocol::close() {
    if (not(send_was_closed || recv_was_closed)) {
        this->skt.shutdown(SHUT_RDWR);
        this->skt.close();
    }
}

std::shared_ptr<States> ClientSide::Protocol::recvStates() {
    StatesTag tag = (StatesTag)recvUint8();

    switch (tag) {
        case StatesTag::GAME_NOT_JOINABLE:
            return std::make_shared<GameNotJoinable>(recvUint8());

        case StatesTag::GAMES_COUNT_L:
            return std::make_shared<GamesCountL>(recvUint8());

        case StatesTag::INFO_GAME_L:
            return recvGameInfo();

        case StatesTag::PLAYER_COUNT_L:
            return std::make_shared<PlayerCountL>(recvUint8());

        case StatesTag::PLAYER_L:
            return std::make_shared<PlayerStateL>(recvBool(), recvUint8());

        case StatesTag::BATTLEFIELD_G:
            return std::make_shared<BattlefieldState>(recvFloat());

        case StatesTag::PLAYER_G:
            return recvPlayerGame();

        case StatesTag::PROJECTILE_COUNT:
            return std::make_shared<ProjectileCount>(recvUint8());

        case StatesTag::PROJECTILE_G:
            return recvProjectileGame();

        case StatesTag::PLAYER_TURN:
            return std::make_shared<PlayerTurn>(recvUint8());

        case StatesTag::WORM_G:
            return recvWormGame();

        case StatesTag::LEVEL_BUILD:
            return recvLevelBuild();

        case StatesTag::CRATE:
            return recvCrate();

        case StatesTag::CRATE_COUNT:
            return std::make_shared<CrateCount>(recvUint8());

        case StatesTag::_YOU_WIN_:
            return std::make_shared<YouWin>(recvUint8());

        default:
            return std::make_shared<PlayerCountL>(recvUint8());
    }
}

float ClientSide::Protocol::meter_to_pixel_x(const float& meter_position) {
    return (meter_position * PPM);
}

float ClientSide::Protocol::meter_to_pixel_y(const float& meter_position) {
    return (720 - meter_position * PPM);
}

void ClientSide::Protocol::recvString64(std::string& str) {
    uint8_t length;
    this->recv(&length, sizeof(uint8_t));

    std::vector<char> str_net;
    str_net.resize(length + 1);  // length + '\0'
    this->recv(str_net.data(), length);
    str = std::string(str_net.data());
}

std::shared_ptr<GameInfoL> ClientSide::Protocol::recvGameInfo() {
    std::string desc, map_name;

    recvString64(desc);
    recvString64(map_name);

    uint8_t p_count = recvUint8();
    uint8_t game_id = recvUint8();

    return std::make_shared<GameInfoL>(desc, map_name, p_count, game_id);
}

std::shared_ptr<PlayerStateG> ClientSide::Protocol::recvPlayerGame() {
    uint8_t id = recvUint8();
    bool currently_on_turn = recvBool();
    bool is_playing = recvBool();
    auto ammo_left = std::make_shared<AmmoLeft>();

    for (size_t i = 0; i < GADGETS_QUANTITY; i++) {
        auto type = (WeaponsAndTools)recvUint8();
        uint8_t ammo = recvUint8();
        ammo_left->weapon_ammo.insert({type, ammo});
    }

    uint8_t avg_life = recvUint8();

    return std::make_shared<PlayerStateG>(is_playing, currently_on_turn,
                                          id, avg_life, std::move(ammo_left));
}

std::shared_ptr<WormStateG> ClientSide::Protocol::recvWormGame() {
    uint8_t id = recvUint8();
    float x = meter_to_pixel_x(recvFloat());
    float y = meter_to_pixel_y(recvFloat());
    bool on_turn_time = recvBool();
    auto equipped_weapon = (WeaponsAndTools)recvUint8();
    bool is_wa = recvBool();
    bool is_jumping = recvBool();
    bool is_backflipping = recvBool();
    bool direction = recvBool();
    bool falling = recvBool();
    float aim_inclination = recvFloat();
    bool charging_weapon = recvBool();
    float life = recvFloat();
    bool drown = recvBool();
    bool using_tool = recvBool();
    uint8_t team = recvUint8();


    return std::make_shared<WormStateG>(id, x, y, equipped_weapon, on_turn_time, is_wa, is_jumping,
                                        is_backflipping, direction, falling, aim_inclination,
                                        charging_weapon, life, drown, using_tool, team);
}

std::shared_ptr<ProjectileStateG> ClientSide::Protocol::recvProjectileGame() {
    uint8_t id = recvUint8();
    float x = meter_to_pixel_x(recvFloat());
    float y = meter_to_pixel_y(recvFloat());
    float time_remaining_till_detonation = recvFloat();
    float angle = recvFloat();
    auto proyectile_type = (WeaponsAndTools)recvUint8();
    bool impacted = recvBool();
    return std::make_shared<ProjectileStateG>(id, x, y, time_remaining_till_detonation,
                                              proyectile_type, impacted, angle);
}

std::shared_ptr<LevelStateG> ClientSide::Protocol::recvLevelBuild() {
    std::string map_name;
    recvString64(map_name);
    uint8_t amount_of_bars = recvUint8();
    std::vector<BarDto> bars;
    for (size_t i = 0; i < amount_of_bars; i++) {
        bars.push_back({(TerrainActors)recvUint8(), meter_to_pixel_x(recvFloat()),
                        meter_to_pixel_y(recvFloat()), recvFloat()});
    }
    return (std::make_shared<LevelStateG>(map_name, amount_of_bars, std::move(bars)));
}

std::shared_ptr<CrateState> ClientSide::Protocol::recvCrate() {
    float x = meter_to_pixel_x(recvFloat());
    float y = meter_to_pixel_y(recvFloat());
    bool falling = recvBool();
    bool was_opened = recvBool();
    _CrateType_ type = (_CrateType_)recvUint8();
    uint8_t id = recvUint8();
    return std::make_shared<CrateState>(x, y, falling, was_opened, type, id);
}
