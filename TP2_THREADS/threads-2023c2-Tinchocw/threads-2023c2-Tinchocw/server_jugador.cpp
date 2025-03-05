#include "server_jugador.h"

Jugador::Jugador(Socket&& jugador, Monitor_de_queues& lobby):
        chats(SIZE_QUEUE), protocolo(std::move(jugador)), lista_de_chats(lobby) {

    sender = new Sender(protocolo, chats);
    receiver = new Receiver(protocolo, lobby, chats);
}

void Jugador::start() {
    lista_de_chats.comunicar_entrada_de_nuevo_jugador(&chats);
    receiver->start();
    sender->start();
}

bool Jugador::estoy_conectado() { return protocolo.estoy_conectado(); }

void Jugador::kill() {
    if (protocolo.estoy_conectado()) {
        protocolo.desconectar_jugador();
    }
    receiver->kill();
    sender->kill();
    chats.close();
}

void Jugador::join() {
    receiver->join();
    sender->join();
}

Jugador::~Jugador() {
    join();
    delete receiver;
    delete sender;
}
