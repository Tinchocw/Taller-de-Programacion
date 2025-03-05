#include "sparser.h"

#include "../common/const.h"

#include "game_browser.h"
#include "player_action.h"
#include "sprotocol.h"

std::shared_ptr<PlayerAction> ServerSide::Parser::makePlayerAction(const Actions& c,
                                                                   ServerSide::Protocol& protocol,
                                                                   const uint8_t id) {
    switch (c) {
        case Actions::START_MOVING:
            return std::make_shared<StartMoving>(protocol, id);

        case Actions::STOP_MOVING:
            return std::make_shared<StopMoving>(id);

        case Actions::JUMP:
            return std::make_shared<Jump>(protocol, id);

        case Actions::ADS_ANGLE:
            return std::make_shared<ADSAngle>(protocol, id);

        case Actions::STOP_ADS_ANGLE:
            return std::make_shared<StopADSAngle>(id);

        case Actions::FIRE_POWER:
            return std::make_shared<FirePower>(id);

        case Actions::SHOOT:
            return std::make_shared<Shoot>(id);

        case Actions::DELAY:
            return std::make_shared<Delay>(protocol, id);

        case Actions::USE_CLICKABLE:
            return std::make_shared<UseClickable>(protocol, id);

        case Actions::CHANGE_WEAPON_OR_TOOL:
            return std::make_shared<ChangeGadget>(protocol, id);

        case Actions::WW3:
            return std::make_shared<WW3Cheat>(id);

        case Actions::RANDOM_KILL:
            return std::make_shared<NoWindCheat>(id);

        case Actions::INFINITE_AMMO:
            return std::make_shared<InfiniteAmmoCheat>(id);

        case Actions::MADNESS:
            return std::make_shared<SupplyRunCheat>(id);

        case Actions::IMMORTAL_WORM:
            return std::make_shared<ImmortalWorm>(id);

        case Actions::MULTIPLE_JUMP:
            return std::make_shared<MultipleJumpCheat>(id);

        case Actions::INFINITE_TURN:
            return std::make_shared<InfiniteTurnCheat>(id);

        case Actions::EVERYONE_1HP:
            return std::make_shared<Everyone1HPCheat>(id);

        default:
            return std::make_shared<NullCommand>();
    }
}

std::shared_ptr<LobbyAction> ServerSide::Parser::makeLobbyAction(
        const Actions& c, ServerSide::Protocol& protocol, GameBrowser& browser, int16_t& game_id,
        const uint8_t& id, Queue<std::shared_ptr<States>>& state_queue) {

    switch (c) {
        case Actions::CREATE:
            return std::make_shared<Create>(browser, game_id, id, state_queue, protocol);

        case Actions::JOIN:
            protocol.recvGameID(game_id);
            return std::make_shared<Join>(browser, game_id, id, state_queue);

        case Actions::READY:
            return std::make_shared<Ready>(browser, id, game_id);

        case Actions::SHOW_GAMES:
            return std::make_shared<ShowGames>(browser, state_queue);

        case Actions::EXIT_GAME:
            return std::make_shared<ExitGame>(browser, id, game_id);

        default:
            return std::make_shared<NullCommand>();
    }
}
