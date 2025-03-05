#include <exception>

#include <string.h>

#include "common_sockets.h"
#include "server_servidor.h"

int main(int argc, char* argv[]) {
    const char* servname = NULL;
    const char* escenarios = NULL;

    if (argc == 2) {
        servname = argv[1];
    } else if (argc == 3) {
        servname = argv[1];
        escenarios = argv[2];
    } else {
        Printer printer;
        printer.mostrar_error_input_servidor(argv[0]);
        return -1;
    }

    Socket srv(servname);
    Socket cliente = srv.accept();

    Servidor servidor(std::move(cliente), escenarios);
    servidor.procesar_juego();

    return 0;
}
