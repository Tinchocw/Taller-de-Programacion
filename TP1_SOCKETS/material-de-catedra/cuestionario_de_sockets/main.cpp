#include <iostream>
#include <sys/socket.h>

bool enviar_mensaje(int fd, char* buf) {
    int ret = send(fd, buf, 16, 0); // No esta definido, tendria que implementar la función
    if (ret == -1)
        return false; // hubo un error

    return true; // el mensaje fue enviado
}

int main() {
    bool enviado = enviar_mensaje();
    return 0;
}
