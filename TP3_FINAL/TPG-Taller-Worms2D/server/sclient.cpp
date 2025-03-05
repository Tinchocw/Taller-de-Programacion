#include "sclient.h"

#include <utility>

#include <spdlog/spdlog.h>

#include "game_loop.h"


ServerSide::Client::Client(Socket&& peer, GameBrowser& browser, const uint8_t& id):
        id(id),
        protocol(std::move(peer)),
        recv(this->protocol, browser, state_queue, this->id),
        send(this->protocol, state_queue),
        killed(false) {
    spdlog::get("server")->debug("Iniciando receptor en cliente {:d}", this->id);
    recv.start();
    spdlog::get("server")->debug("Iniciando sender en cliente {:d}", this->id);
    send.start();
}

bool ServerSide::Client::isAlive() { return (recv.is_alive() && send.is_alive()); }

void ServerSide::Client::stop() {
    recv.stop();
    send.stop();
}

void ServerSide::Client::kill() {
    spdlog::get("server")->debug("Matando cliente {:d}", id);
    killed = true;
    this->stop();
    state_queue.close();
    protocol.close();
}

ServerSide::Client::~Client() {
    if (!killed) {
        this->kill();
    }
    spdlog::get("server")->debug("Joineando receptor en cliente {:d}", id);
    recv.join();
    spdlog::get("server")->debug("Joineando sender en cliente {:d}", id);
    send.join();
}
