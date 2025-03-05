#ifndef LOBBY_LISTENER_H
#define LOBBY_LISTENER_H

#include <map>
#include <memory>
#include <vector>

#include <stdint.h>

#include "../common/States.h"
#include "../common/queue.h"

class PlayerFrame;
class GameFrame;

class LobbyListener {

public:
    /*
        @param games: Referencia al vector de game frames
        @param lobby_states: Referencia a la queue de lobby states del eventloop
        @param games_q: Cantidad de games a popear

        @brief Setea la informacion que recibe de los states a los game frames
    */
    static void setGameFrames(std::vector<std::unique_ptr<GameFrame>>& games,
                              Queue<std::shared_ptr<States>>& lobby_states, const uint8_t& games_q);
    /*
        @param lobby_states: Referencia a la queue de lobby states del eventloop

        @brief Retorna la cantidad de game frames a crear
    */
    static uint8_t getGameFramesQuantity(Queue<std::shared_ptr<States>>& lobby_states);
    /*
        @param lobby_states: Referencia a la queue de lobby states del eventloop

        @brief Retorna la cantidad de jugadores en el lobby
    */
    static uint8_t getPlayersInLobbyQuantity(Queue<std::shared_ptr<States>>& lobby_states);
    /*
        @param players: Referencia al mapa de jugadores
        @param lobby_states: Referencia a la queue de lobby states del eventloop
        @param players_q: Cantidad de jugadores en lobby

        @brief Setea la informacion que recibe de los states a los player frame
    */
    static void setPlayers(std::map<uint8_t, std::unique_ptr<PlayerFrame>>& players,
                           Queue<std::shared_ptr<States>>& lobby_states, const uint8_t& players_q);
};


#endif
