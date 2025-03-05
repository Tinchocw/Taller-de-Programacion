#include "server_servidor.h"

Servidor::Servidor(Socket&& acept_): acept(std::move(acept_)) { aceptador = new Aceptador(acept); }

void Servidor::run() {
    aceptador->start();
    std::string mensaje;
    while (std::cin >> mensaje && mensaje != "q") {}
    aceptador->kill();
}
Servidor::~Servidor() {
    acept.shutdown(2);
    acept.close();
    aceptador->join();
    delete aceptador;
}
