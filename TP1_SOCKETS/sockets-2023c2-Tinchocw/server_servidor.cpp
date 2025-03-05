#include "server_servidor.h"

Servidor::Servidor(Socket&& cliente, std::string const escenarios):
        protocolo(std::move(cliente)), juego(escenarios) {}

void Servidor::procesar_juego() {
    bool was_closed = false;
    while (not was_closed) {
        char codigo_mensaje;
        std::string argumento;
        std::vector<std::string> respuesta_juego;

        protocolo.recibir_instrucciones(codigo_mensaje, argumento, was_closed);
        if (not was_closed) {
            controlador_de_comandos.controlar_comandos_a_juego(juego, codigo_mensaje, argumento);
            controlador_de_comandos.controlar_respuesta_de_juego(juego, codigo_mensaje,
                                                                 respuesta_juego);
            protocolo.enviar_respuesta(codigo_mensaje, respuesta_juego);
            printer.mostrar_coordenada_actual(juego.coordenada_actual_gusano());
        }
    }
}
