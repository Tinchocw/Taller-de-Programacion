#ifndef C_GAME_H
#define C_GAME_H

#include <QApplication>
#include <QThread>
#include <memory>

#include "../common/logger.h"

#include "cheatmenu.h"
#include "client_event_loop.h"
#include "mainwindow.h"

#define HOSTNAME argv[1]
#define SERVNAME argv[2]


class Game {
private:
    std::unique_ptr<QApplication> app;
    std::unique_ptr<MainWindow> window;
    bool initGame;
    std::unique_ptr<CheatMenu> cheat_menu;
    EventLoop client;

public:
    /*
        @brief Crea el eventloop y las ventanas de qt
    */
    Game(int& argc, char* argv[]);
    /*
        @brief Corre el menu de qt y luego inicia el eventloop si se ingreso a un game. Retorna el
       output de qt.
    */
    int run();
    /*
        @brief Joinea el eventloop si se ingreso a un game.
    */
    ~Game();
};


#endif
