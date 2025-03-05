#include "client_interprete.h"

void Interprete::interpretar_input(const std::string& comando, const std::string& argumento,
                                   struct mensaje& msj) {
    if (comando == CHAT_INPUT) {
        msj.comando = CHAT;
        msj.argumento = argumento;
    } else if (comando == READ_INPUT) {
        msj.comando = READ;
        msj.argumento = argumento;
    } else if (comando == EXIT_INPUT) {
        msj.comando = EXIT;
    }
}
