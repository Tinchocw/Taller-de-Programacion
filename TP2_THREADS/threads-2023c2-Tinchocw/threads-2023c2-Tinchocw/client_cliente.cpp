#include "client_cliente.h"

Cliente::Cliente(const char* hostname, const char* servername): protocolo(hostname, servername) {}

void Cliente::run() {
    std::string comando;
    std::string argumento;

    while (protocolo.esta_conectado()) {
        std::cin >> comando;
        if (protocolo.esta_conectado()) {
            if (comando == CHAT_INPUT || comando == READ_INPUT) {
                getline(std::cin >> std::ws, argumento);
            }
            struct mensaje msj;
            interprete.interpretar_input(comando, argumento, msj);
            ejecutar_input(msj);
        } else {
            protocolo.desconectar_jugador();
        }
    }
}

void Cliente::ejecutar_input(struct mensaje msj) {
    if (msj.comando == CHAT) {
        protocolo.enviar_chat(msj);
    } else if (msj.comando == READ) {
        recibir_mensajes_solicitados(msj);
    } else if (msj.comando == EXIT) {
        protocolo.desconectar_jugador();
    }
}

void Cliente::recibir_mensajes_solicitados(struct mensaje msj) {
    int cantidad_de_lecturas = std::stoi(msj.argumento);
    for (int i = 0; i < cantidad_de_lecturas; i++) {
        protocolo.recibir_instruccion(msj);
        mostrar_mesaje_recibido(msj);
    }
}

void Cliente::mostrar_mesaje_recibido(struct mensaje msj) {

    if (msj.comando == CHAT) {
        Printer::printear_chat(msj.argumento);
    } else if (msj.comando == PLAYER_CANT) {
        Printer::printear_cantidad_de_jugadores(msj.argumento);
    }
}
