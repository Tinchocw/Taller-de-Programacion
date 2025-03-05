#include "server_aceptador.h"


Aceptador::Aceptador(Socket& acept): skt(acept), seguir_aceptando(true) {}
void Aceptador::run() {
    try {
        while (seguir_aceptando) {

            Socket peer = skt.accept();
            auto* jugador = new Jugador(std::move(peer), lista_de_chats);
            jugador->start();

            reap_dead();
            jugadores.push_back(jugador);
        }
    } catch (LibError& error) {
        if (!seguir_aceptando) {
            std::cerr << "El socket aceptador fue cerrado adecuadamente" << std::endl;
        } else {
            std::cerr << "El socket aceptador se cerro de forma inesperada" << std::endl;
        }
        kill_all();
    }
}

void Aceptador::kill() { seguir_aceptando = false; }

void Aceptador::kill_all() {
    for (auto& jugador: jugadores) {
        jugador->kill();
    }
}

void Aceptador::reap_dead() {
    jugadores.remove_if([](Jugador* jugador) {
        if (!jugador->estoy_conectado()) {
            delete jugador;
            return true;
        }
        return false;
    });
}
Aceptador::~Aceptador() {
    for (auto& jugador: jugadores) {
        delete jugador;
    }
    jugadores.clear();
}
