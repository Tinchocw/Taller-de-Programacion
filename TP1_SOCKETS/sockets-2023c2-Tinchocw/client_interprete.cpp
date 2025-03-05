#include "client_interprete.h"

void Interprete::preparar_intruccion_a_enviar(Protocolo_cliente& protocolo,
                                              std::string mensaje_entrada) {

    std::string codigo_instruccion;
    std::string argumento;

    std::istringstream stream(mensaje_entrada);
    stream >> codigo_instruccion >> argumento;

    if (codigo_instruccion == SELECT) {
        protocolo.procesar_select(argumento);
    } else if (codigo_instruccion == DIR) {
        protocolo.procesar_dir(argumento);
    } else if (codigo_instruccion == MOV) {
        protocolo.procesar_mov();
    } else if (codigo_instruccion == JUMP) {
        protocolo.procesar_jump(argumento);
    }
}
