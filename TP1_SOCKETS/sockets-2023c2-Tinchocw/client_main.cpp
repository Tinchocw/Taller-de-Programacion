#include <iostream>

#include "client_cliente.h"

int main(int argc, char* argv[]) {

    const char* hostname = NULL;
    const char* servname = NULL;

    if (argc == 2) {
        hostname = argv[1];
    } else if (argc == 3) {
        hostname = argv[1];
        servname = argv[2];
    } else {
        Printer printer;
        printer.mostrar_error_input_cliente(argv[0]);
        return -1;
    }
    Cliente cliente(hostname, servname);
    cliente.realizar_movimientos();

    return 0;
}
