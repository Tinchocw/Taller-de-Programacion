#ifndef CLIENT_H
#define CLIENT_H

#include <atomic>
#include <list>
#include <memory>

#include <SDL2pp/SDL2pp.hh>
#include <spdlog/spdlog.h>

#include "../common/config.h"
#include "../common/thread.h"

#include "ActorHolder.h"
#include "GameActor.h"
#include "LevelActors.h"
#include "TeamResourcesHolder.h"
#include "TerrainHolder.h"
#include "TexturesPool.h"
#include "Window.h"
#include "audio_player.h"
#include "camera.h"
#include "cprotocol.h"
#include "creceiver.h"
#include "csender.h"
#include "inputHandler.h"
#include "text_printer.h"

#define FRAME_DURATION Config::yamlNode["frame_duration"].as<int>()

class Action;
class States;
class CheatMenu;

class EventLoop: public Thread {

private:
    /// thread specific atributes
    std::atomic<bool> quit;
    std::atomic<bool> my_turn;
    std::atomic<bool> runned;
    std::atomic<bool> mouse_priority;
    std::atomic<bool> kb_priority;
    bool loaded_base_textures;
    int8_t win;
    ClientSide::Protocol protocol;
    ClientSide::Receiver recv;
    ClientSide::Sender send;

    /// Media specific atributes and info for rendering
    Camera camera;
    IHandler input;
    AudioPlayer audio_player;
    std::unique_ptr<CheatMenu>& cheat_menu;

    /// Holders for actors in the game
    TerrainHolder terrain_elements;
    ActorHolder players;
    ActorHolder proyectiles;
    ActorHolder crates;

    /// Queues for the states and actions
    Queue<std::shared_ptr<States>> game_state_queue;
    Queue<std::shared_ptr<States>> lobby_state_queue;
    Queue<std::shared_ptr<Action>> action_queue;

    CameraPriority camera_priority;
    /*
        @param turn_start: Momento de inicio del turno
        @parma txt_pool: Pool de texturas
        @param resources_holder: Texturas segun el equipo

        @brief Procesa los gamestate segun su tag
    */
    void process_game_states(std::chrono::time_point<std::chrono::steady_clock>& turn_start,
                             TexturesPool& txt_pool, TeamResourcesHolder& resources_holder);
    /*
        @param worm: Gamestate del worm

        @brief Centra la camara en el worm si la prioridad lo admite
    */
    void viewWorm(const std::shared_ptr<WormStateG>& worm);
    /*
        @param proj: GameState del proyectil

        @brief Centra la camara en el proyectil si la prioridad lo admite
    */
    void viewProjectile(const std::shared_ptr<ProjectileStateG>& proj);
    /*
        @param loop_start_time: Referencia al momento de inicio del loop del render

        @brief Descansa un intervalo la ui de sdl
    */
    void rest(int& loop_start_time);

public:
    /*
        @brief Construye el eventloop e inicializa los hilos sender y receiver
    */
    explicit EventLoop(const char* hostname, const char* servname,
                       std::unique_ptr<CheatMenu>& cheat_menu);
    /*
        @brief Corre el eventloop, iniciando la ventana de sdl y el input handler
    */
    void run() override;
    /*
        @brief Joinea los hilos y libera la memoria
    */
    ~EventLoop() override;

    friend class MainWindow;
    friend class GameFrame;
    friend class CheatMenu;
};

#endif
