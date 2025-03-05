#include "common_printer.h"


void Printer::mostrar_coordenada_actual(std::vector<std::string> movimientos) {
    std::cout << movimientos.at(0) << " " << movimientos.at(1) << std::endl;
}

void Printer::mostrar_mensaje_de_error() { std::cout << "Fallo" << std::endl; }

void Printer::mostrar_error_input_cliente(const char* ejecutable) {
    std::cerr << "Bad program call. Expected " << ejecutable << " <hostname> [<servname>]"
              << std::endl;
}
void Printer::mostrar_error_input_servidor(const char* ejecutable) {
    std::cerr << "Bad program call. Expected " << ejecutable << " <servname> <escenario> "
              << std::endl;
}
