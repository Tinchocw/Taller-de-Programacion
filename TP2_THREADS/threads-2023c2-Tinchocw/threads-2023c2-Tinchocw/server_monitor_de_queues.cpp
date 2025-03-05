#include "server_monitor_de_queues.h"

void Monitor_de_queues::agregar_queue(Queue* jugador) { jugadores.push_back(jugador); }

void Monitor_de_queues::eliminar_jugador(Queue* jugador) { jugadores.remove(jugador); }

void Monitor_de_queues::broadcastear_mensaje(struct mensaje msj) {
    std::unique_lock<std::mutex> lck(m);
    for (auto& jugador: jugadores) {
        jugador->push(msj);
    }
    Printer::printear_chat(msj.argumento);
}
void Monitor_de_queues::broadcastear_cantidad_jugadores() {
    struct mensaje cant_jugadores;
    cant_jugadores.comando = PLAYER_CANT;
    cant_jugadores.argumento = std::to_string(size());

    for (auto& jugador: jugadores) {
        jugador->push(cant_jugadores);
    }
}


int Monitor_de_queues::size() { return jugadores.size(); }

void Monitor_de_queues::comunicar_entrada_de_nuevo_jugador(Queue* jugador) {
    std::unique_lock<std::mutex> lck(m);
    agregar_queue(jugador);
    broadcastear_cantidad_jugadores();
    Printer::printear_cantidad_de_jugadores(std::to_string(size()));
}

void Monitor_de_queues::comunicar_salida_de_jugador(Queue* jugador) {
    std::unique_lock<std::mutex> lck(m);
    eliminar_jugador(jugador);
    broadcastear_cantidad_jugadores();
    Printer::printear_cantidad_de_jugadores(std::to_string(size()));
}
