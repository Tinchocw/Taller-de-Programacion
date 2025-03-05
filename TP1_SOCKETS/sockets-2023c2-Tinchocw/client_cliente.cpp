#include "client_cliente.h"

Cliente::Cliente(const std::string& hostname, const std::string& servname):
        protocolo(hostname, servname) {}
void Cliente::realizar_movimientos() {
    std::string instruccion;
    while (getline(std::cin, instruccion)) {

        if (!instruccion.empty() && instruccion[0] != '#') {

            interprete.preparar_intruccion_a_enviar(protocolo, instruccion);
            std::vector<std::string> respuesta = protocolo.instruccion_recibida();
            mostrar_respuesta_a_instruccion(respuesta);
        }
    }
}

void Cliente::mostrar_respuesta_a_instruccion(std::vector<std::string> respuesta) {
    if (respuesta.at(0) == FALLO) {
        printer.mostrar_mensaje_de_error();
    } else {
        printer.mostrar_coordenada_actual(respuesta);
    }
}
