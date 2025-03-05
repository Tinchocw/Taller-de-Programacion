#include "server.h"

#include <spdlog/spdlog.h>

Server::Server(char* servname): acceptor(servname) {
    spdlog::get("server")->debug("Iniciando hilo aceptador");
    acceptor.start();
    spdlog::get("server")->debug("Hilo aceptador iniciado con exito");
}

void Server::run() {
    char command;
    while (std::cin >> command) {
        if (command == 'q') {
            break;
        }
    }
}

Server::~Server() {
    acceptor.kill();
    spdlog::get("server")->debug("Joineando hilo aceptador");
    acceptor.join();
}
