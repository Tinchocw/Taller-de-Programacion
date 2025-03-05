#include <iostream>

#include "common_sockets.h"
#include "server_aceptador.h"
#include "server_servidor.h"

int main(int argc, char* argv[]) {
    const char* servname = NULL;

    if (argc == 2) {
        servname = argv[1];
    } else {
        return -1;
    }

    Socket acept(servname);
    Servidor servidor(std::move(acept));
    servidor.run();


    return 0;
}
