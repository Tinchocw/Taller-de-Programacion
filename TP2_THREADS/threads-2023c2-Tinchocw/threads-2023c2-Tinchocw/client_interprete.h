#ifndef THREADS_2023C2_TINCHOCW_CLIENT_INTERPRETE_H
#define THREADS_2023C2_TINCHOCW_CLIENT_INTERPRETE_H

#include <iostream>
#include <sstream>
#include <string>

#include "client_protocol.h"
#include "common_constantes_protocolo.h"

namespace {  // NOLINT
const char CHAT_INPUT[] = "Chat";
const char READ_INPUT[] = "Read";
const char EXIT_INPUT[] = "Exit";
}  // namespace


class Interprete {
public:
    void interpretar_input(const std::string& comando, const std::string& argumento,
                           struct mensaje& msj);
};

#endif  // THREADS_2023C2_TINCHOCW_CLIENT_INTERPRETE_H
