#include "game.h"

Game::Game(int& argc, char* argv[]):
        app(std::make_unique<QApplication>(argc, argv)),
        window(std::make_unique<MainWindow>(client, initGame)),
        initGame(false),
        cheat_menu(nullptr),
        client(HOSTNAME, SERVNAME, cheat_menu) {}

int Game::run() {
    window->show();
    int ret = app->exec();

    if (ret != 0) {
        return ret;
    }

    if (initGame) {
        // window ya esta cerrado si el thread esta aca
        window = nullptr;
        client.start();
        cheat_menu->show();
        ret = app->exec();
    }

    return ret;
}

Game::~Game() {
    if (initGame) {
        client.join();
    }
}
