#include "server_sender.h"

Sender::Sender(Protocolo_servidor& protocolo_, Queue& queue_):
        protocolo(protocolo_), queue(queue_), estoy_vivo(true) {}
void Sender::run() {
    try {
        while (protocolo.estoy_conectado() && estoy_vivo) {
            auto mensaje = queue.pop();
            protocolo.enviar_mensaje(mensaje);
        }
        kill();
    } catch (std::runtime_error& error) {
        if (!estoy_vivo) {
            std::cerr << "Se cerro de forma correcta la chats" << std::endl;
            std::cerr << "Estoy trabado en el pop y se cierra la chats" << std::endl;
        }
    }
}
void Sender::kill() { estoy_vivo = false; }
