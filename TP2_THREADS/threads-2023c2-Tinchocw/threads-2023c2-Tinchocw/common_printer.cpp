#include "common_printer.h"

void Printer::printear_chat(const std::string& mensaje) { std::cout << mensaje << std::endl; }

void Printer::printear_cantidad_de_jugadores(const std::string& mensaje) {
    std::cout << "Jugadores " << mensaje << ", esperando al resto de tus amigos..." << std::endl;
}
