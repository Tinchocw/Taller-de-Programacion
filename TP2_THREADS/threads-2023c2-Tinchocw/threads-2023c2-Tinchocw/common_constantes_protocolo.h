#ifndef THREADS_2023C2_TINCHOCW_COMMON_CONSTANTES_PROTOCOLO_H
#define THREADS_2023C2_TINCHOCW_COMMON_CONSTANTES_PROTOCOLO_H

#include <iostream>
#include <string>
namespace {  // NOLINT
const char CHAT = 'C';
const char READ = 'R';
const char EXIT = 'E';

const char PLAYER_CANT = 'P';

const char CHAT_NET_CLIENT = '\005';
const char CHAT_NET_SERVER = 9;
const char PLAYER_CANT_NET = '\006';

const int ONE_BYTE = 1;
const int TWO_BYTES = 2;
}  // namespace

struct mensaje {
    // cppcheck-suppress unusedStructMember
    char comando = 0;
    std::string argumento;
};


#endif  // THREADS_2023C2_TINCHOCW_COMMON_CONSTANTES_PROTOCOLO_H
